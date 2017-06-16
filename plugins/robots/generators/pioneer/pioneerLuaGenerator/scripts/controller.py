#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import binascii
import math
import serial
import signal
import socket
import struct
import sys
import threading
import time

debugEnabled = False
verboseEnabled = False
textMutex = threading.Lock()

def debug(text):
    global debugEnabled
    global textMutex

    if debugEnabled:
        textMutex.acquire()
        print("[%.6f] " % time.time() + text)
        textMutex.release()

def verbose(text):
    global verboseEnabled
    global textMutex

    if verboseEnabled:
        textMutex.acquire()
        print("[%.6f] " % time.time() + text)
        textMutex.release()


class SerialHandler:
    def __init__(self, ports, callback=None):
        if "ip" in ports.keys():
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            serverAddress, serverPort = "127.0.0.1", 5500
            modemAddress, modemPort = 1, 2

            parts = ports["ip"].split(":")
            if len(parts) == 1:
                serverAddress = parts[0]
            else:
                serverAddress, serverPort = parts[0], int(parts[1])

            if "modem" in ports.keys():
                parts = ports["modem"].split(":")
                modemAddress, modemPort = int(parts[0]), int(parts[1])

            print("Connecting to %s:%u, modem %u:%u" % (serverAddress, serverPort, modemAddress, modemPort))
            self.socket.connect((serverAddress, serverPort))
            self.socket.settimeout(0.1)
            #self.socket.sendall("%u:%u\n" % (modemAddress, modemPort))
            self.socket.sendall("%u\n" % modemAddress)
            self.net = True
        elif "serial" in ports.keys():
            self.socket = serial.Serial()
            self.socket.port     = ports["serial"]
            self.socket.baudrate = 57600
            self.socket.parity   = "N"
            self.socket.rtscts   = False
            self.socket.xonxoff  = False
            self.socket.timeout  = 0.1
            try:
                self.socket.open()
            except serial.SerialException, e:
                print("Could not open serial port %s: %s" % (self.socket.portstr, e))
                exit()
            self.net = False
        else:
            print("No connection types specified")
            exit()

        self.callback = callback
        self.parser = PacketParser()

        self.handler = threading.Thread(target=self.thread)
        self.stopRequest = False
        self.handler.start()

    def send(self, message):
        data = PacketParser.create(message[0], message[1])
        if self.net:
            self.socket.sendall(data)
        else:
            self.socket.write(data)
        debug("TX: class %02X, length %u" % (message[0], len(message[1])))

    def sendRaw(self, message):
        if self.net:
            self.socket.sendall(message)
        else:
            self.socket.write(message)

    def stop(self):
        self.stopRequest = True

    def thread(self):
        while not self.stopRequest:
            try:
                data = self.socket.recv(64) if self.net else self.socket.read()
            except serial.SerialException:
                print("Serial port error")
                exit()
            except:
                continue

            while len(data) > 0:
                count = self.parser.process(data)
                data = data[count:]

                if self.parser.state == PacketParser.STATE_DONE:
                    message = (self.parser.packet.id, self.parser.packet.buffer)
                    debug("RX: class %02X, length %u" % (message[0], len(message[1])))
                    if self.callback is not None:
                        self.callback(message)
        self.socket.close()


class PacketParser:
    STATE_SYNC_A, STATE_SYNC_B, STATE_ID, STATE_LENGTH, STATE_PAYLOAD, STATE_CHECKSUM_A, STATE_CHECKSUM_B, \
            STATE_DONE, STATE_ERROR = range(0, 9)

    class Packet:
        def __init__(self):
            self.buffer = ""
            self.id = 0

    def __init__(self):
        self.reset()
        self.counters = {"received": 0, "errors": 0}

    def reset(self):
        self.state = PacketParser.STATE_SYNC_A
        self.packet = PacketParser.Packet()
        self.length = 0
        self.position = 0
        self.checksum = (0, 0)

    def process(self, data):
        i = 0
        while i < len(data):
            if self.state == PacketParser.STATE_DONE or self.state == PacketParser.STATE_ERROR:
                self.reset()
            value = data[i]
            i += 1

            if self.state == PacketParser.STATE_SYNC_A:
                if value == 'p':
                    self.state = PacketParser.STATE_SYNC_B
            elif self.state == PacketParser.STATE_SYNC_B:
                if value == 'l':
                    self.checksum = (0, 0)
                    self.position = 0
                    self.state = PacketParser.STATE_ID
                else:
                    self.state = PacketParser.STATE_SYNC_A
            elif self.state == PacketParser.STATE_ID:
                self.checksum = PacketParser.getChecksum(self.checksum, [ord(value)])
                self.packet.buffer = "" #Reset packet buffer
                self.packet.id = ord(value)
                self.state = PacketParser.STATE_LENGTH;
            elif self.state == PacketParser.STATE_LENGTH:
                self.checksum = PacketParser.getChecksum(self.checksum, [ord(value)])
                self.length = ord(value)
                self.state = PacketParser.STATE_PAYLOAD if self.length > 0 else PacketParser.STATE_CHECKSUM_A
            elif self.state == PacketParser.STATE_PAYLOAD:
                self.checksum = PacketParser.getChecksum(self.checksum, [ord(value)])
                self.packet.buffer += value
                self.length -= 1
                if self.length == 0:
                    self.state = PacketParser.STATE_CHECKSUM_A
            elif self.state == PacketParser.STATE_CHECKSUM_A:
                if self.checksum[0] == ord(value):
                    self.state = PacketParser.STATE_CHECKSUM_B
                else:
                    self.state = PacketParser.STATE_ERROR
                    self.counters["errors"] += 1
                    break
            elif self.state == PacketParser.STATE_CHECKSUM_B:
                if self.checksum[1] == ord(value):
                    self.state = PacketParser.STATE_DONE
                    self.counters["received"] += 1
                    break
                else:
                    self.state = PacketParser.STATE_ERROR
                    self.counters["errors"] += 1
                    break
        return i

    @staticmethod
    def create(identifier, payload):
        buf = "pl"
        crc = PacketParser.getChecksum((0, 0), [identifier])
        buf += chr(identifier)
        crc = PacketParser.getChecksum(crc, [len(payload)])
        buf += chr(len(payload))

        for value in payload:
            crc = PacketParser.getChecksum(crc, [ord(value)])
            buf += value

        buf += chr(crc[0])
        buf += chr(crc[1])
        return buf

    @staticmethod
    def getChecksum(previous, data):
        for value in data:
            b1 = (previous[0] + value) & 0xFF
            b2 = (previous[1] + b1) & 0xFF
        return (b1, b2)


class Messenger:
    SET_MODE                       = 0x0B
    AP_TEXT_LOG                    = 0x10
    BOOT                           = 0x12
    SYSTEM_COMMAND                 = 0x36

    COMPONENT_COUNT_REQUEST        = 0x29
    COMPONENT_COUNT                = 0x2A
    COMPONENT_INFO_REQUEST         = 0x2B
    COMPONENT_INFO                 = 0x2C
    COMPONENT_INFO_ERROR           = 0x2D
    COMPONENT_FIELD_REQUEST        = 0x2E
    COMPONENT_FIELD                = 0x2F
    COMPONENT_FIELD_RESPONSE       = 0x30
    COMPONENT_FIELD_DESCRIBE       = 0x31
    COMPONENT_FIELD_INFO           = 0x32
    COMPONENT_FIELD_ERROR          = 0x33

    COMPONENT_MESSAGE              = 0x38

    COMPONENT_MULTI_FIELD_REQUEST  = 0x39
    COMPONENT_MULTI_FIELD          = 0x3A
    COMPONENT_MULTI_FIELD_RESPONSE = 0x3B

    COMPONENT_FILE_DESCRIBE        = 0x3C
    COMPONENT_FILE_INFO            = 0x3D
    COMPONENT_FILE_INFO_ERROR      = 0x3E
    COMPONENT_FILE_WRITE           = 0x3F
    COMPONENT_FILE_WRITE_RESPONSE  = 0x40
    COMPONENT_FILE_READ            = 0x41
    COMPONENT_FILE_READ_RESPONSE   = 0x42
    COMPONENT_FILE_READ_ERROR      = 0x43

    REQ_PASS                       = 0x08
    PASS_INFO                      = 0x09
    PASS                           = 0x0A
    MEDIA_PASSPORT                 = 0x44


    class ComponentField:
        def __init__(self, index, fieldType, name, scale, unit, minimal, maximal):
            self.index = index
            self.type = fieldType
            self.name = name
            self.scale = scale
            self.unit = unit
            self.minimal = minimal
            self.maximal = maximal


    class Component:
        def __init__(self, index, componentType, minor, revision, hash, name, fieldCount):
            self.index = index
            self.type = componentType
            self.minor = minor
            self.revision = revision
            self.hash = hash
            self.name = name
            self.fieldCount = fieldCount
            self.fields = {}


    class Hub:
        def __init__(self):
            self.componentCount = 0
            self.components = {}
            self.mutex = threading.Lock()


    def __init__(self, ports, deviceFilter):
        self.fragmentation = (320, 40)
        self.stopRequest = False
        self.comparator = None
        self.event = (False, False)
        self.hub = Messenger.Hub()
        self.deviceFilter = deviceFilter
        self.serhan = SerialHandler(ports, self.callback)
        self.passports = []

    @staticmethod
    def typeToString(value):
        TYPES = {0: "bool", 1: "char", 2: "int8", 3: "int16", 4: "int32", 5: "int64",\
                6: "uint8", 7: "uint16", 8: "uint32", 9: "uint64", 10: "float", 11: "double"}
        return "undefined" if value not in TYPES.keys() else TYPES[value]

    @staticmethod
    def typeToSize(value):
        # Pure magic, see protocol documentation
        TYPES = {0: 1, 1: 1, 2: 1, 3: 2, 4: 4, 5: 8, 6: 1, 7: 2, 8: 4, 9: 8, 10: 4, 11: 8}
        return 0 if value not in TYPES.keys() else TYPES[value]

    @staticmethod
    def fetchValue(valueType, array):
        if Messenger.typeToSize(valueType) > len(array):
            return None
        result = None
        if valueType == 0: # bool
            result = True if ord(array[0]) != 0 else False
        elif valueType == 1: # char
            result = struct.unpack("c", array[0:1])[0]
        elif valueType == 2: # int8
            result = struct.unpack("b", array[0:1])[0]
        elif valueType == 3: # int16
            result = struct.unpack("<h", array[0:2])[0]
        elif valueType == 4: # int32
            result = struct.unpack("<i", array[0:4])[0]
        elif valueType == 5: # int64
            result = struct.unpack("<q", array[0:8])[0]
        elif valueType == 6: # uint8
            result = struct.unpack("B", array[0:1])[0]
        elif valueType == 7: # uint16
            result = struct.unpack("<H", array[0:2])[0]
        elif valueType == 8: # uint32
            result = struct.unpack("<I", array[0:4])[0]
        elif valueType == 9: # uint64
            result = struct.unpack("<Q", array[0:8])[0]
        elif valueType == 10: # float
            result = struct.unpack("<f", array[0:4])[0]
        elif valueType == 11: # double
            result = struct.unpack("<d", array[0:8])[0]
        return result

    @staticmethod
    def createValue(valueType, value):
        result = None
        if valueType == 0: # bool
            result = "\x01" if value in (True, 1, "True", "true", "1", "on") else "\x00"
        elif valueType == 1: # char
            result = struct.pack("c", value[0])
        elif valueType == 2: # int8
            result = struct.pack("b", int(value))
        elif valueType == 3: # int16
            result = struct.pack("<h", int(value))
        elif valueType == 4: # int32
            result = struct.pack("<i", int(value))
        elif valueType == 5: # int64
            result = struct.pack("<q", int(value))
        elif valueType == 6: # uint8
            result = struct.pack("B", int(value))
        elif valueType == 7: # uint16
            result = struct.pack("<H", int(value))
        elif valueType == 8: # uint32
            result = struct.pack("<I", int(value))
        elif valueType == 9: # uint64
            result = struct.pack("<Q", int(value))
        elif valueType == 10: # float
            result = struct.pack("<f", float(value))
        elif valueType == 11: # double
            result = struct.pack("<d", float(value))
        return result

    def processComponentMessage(self, message):
        msgComponent = ord(message[1][0])
        msgType = ord(message[1][1])
        if msgComponent not in self.deviceFilter:
            return
        self.hub.mutex.acquire()
        if msgComponent in self.hub.components.keys() and len(self.hub.components[msgComponent].fields) > 0:
            component = self.hub.components[msgComponent]
            offset = 2
            print("[%.6f] message from component %u, type %u" % (time.time(), msgComponent, msgType))
            maxNameLength = max(map(lambda x: len(x.name), component.fields.values()))
            while offset < len(message[1]):
                field = ord(message[1][offset])
                if field not in component.fields.keys():
                    debug("\tError during processing field %u" % field)
                    break
                value = Messenger.fetchValue(component.fields[field].type, message[1][offset + 1:])
                print("\tField: %s%s:" % (' ' * (maxNameLength - len(component.fields[field].name)),\
                        component.fields[field].name)),
                print(value)
                offset += 1 + Messenger.typeToSize(component.fields[field].type)
        else:
            print("Message from an unknown component %u, type %u" % (msgComponent, msgType))
        self.hub.mutex.release()

    def processTextLogMessage(self, message):
        prefix = ["DEBUG", "INFO", "NOTICE", "WARNING", "ERROR", "ALERT"]
        msgTimestamp = struct.unpack("<I", message[1][0:4])[0]
        msgLevel = ord(message[1][4])
        msgText = message[1][5:]
        verbose("Debug message: [%.3f] %s: %s" % (float(msgTimestamp) / 1000.0, prefix[msgLevel], msgText))

    def processComponentCount(self, message):
        values = self.parseComponentCount(message)
        verbose("Component count received: count %u" % values["count"])

    def processMediaPassport(self, message):
        def dist(a, b):
            ONE_DEGREE_LONG = 111195.0
            dx = (-a[1] + b[1]) * ONE_DEGREE_LONG * math.cos(a[0] * math.pi / 180.0)
            dy = (-a[0] + b[0]) * ONE_DEGREE_LONG
            return math.sqrt(dx * dx + dy * dy)

        msgComponent = ord(message[1][0])
        msgNumber = struct.unpack("<H", message[1][1:3])[0]
        fields = {}
        fields["component"] = msgComponent
        fields["number"] = msgNumber

        position = 3
        while position < len(message[1]):
            typeId = ord(message[1][position])
            position += 1
            if typeId == 0x00: # Time
                if position + 8 > len(message[1]):
                    print("Error in passport structure")
                    break
                else:
                    fields["time"] = struct.unpack("<Q", message[1][position:position + 8])[0]
                    position += 8
            elif typeId == 0x01: # Position
                if position + 12 > len(message[1]):
                    print("Error in passport structure")
                    break
                else:
                    lat = struct.unpack("<i", message[1][position + 0:position + 4])[0]
                    lon = struct.unpack("<i", message[1][position + 4:position + 8])[0]
                    alt = struct.unpack("<i", message[1][position + 8:position + 12])[0]
                    fields["position"] = (float(lat) / 1e7, float(lon) / 1e7, float(alt) / 1e3)
                    position += 12
            elif typeId == 0x02: # Barometric altitude
                if position + 4 > len(message[1]):
                    print("Error in passport structure")
                    break
                else:
                    alt = struct.unpack("<i", message[1][position:position + 4])[0]
                    fields["altitude"] = float(alt) / 1e3
                    position += 4
            elif typeId == 0x03: # Orientation
                if position + 6 > len(message[1]):
                    print("Error in passport structure")
                    break
                else:
                    roll = struct.unpack("<h", message[1][position + 0:position + 2])[0]
                    pitch = struct.unpack("<h", message[1][position + 2:position + 4])[0]
                    yaw = struct.unpack("<h", message[1][position + 4:position + 6])[0]
                    fields["orientation"] = (float(roll) / 1e2, float(pitch) / 1e2, float(yaw) / 1e2)
                    position += 6
            elif typeId == 0x04: # Illumination
                if position + 4 > len(message[1]):
                    print("Error in passport structure")
                    break
                else:
                    value = struct.unpack("<i", message[1][position + 0:position + 4])[0]
                    if "illumination" not in fields.keys():
                        fields["illumination"] = []
                    fields["illumination"].append(float(value))
                    position += 4
        if len(self.passports) > 0 and "position" in self.passports[-1].keys() and "position" in fields.keys():
            distance = dist((self.passports[-1]["position"][0], self.passports[-1]["position"][1]),
                    (fields["position"][0], fields["position"][1]))
            dt = fields["time"] - self.passports[-1]["time"]
            verbose("Distance from previous passport: %f, time delta: %f" % (distance, dt))
        self.passports.append(fields)
        verbose(str(fields))
        return fields

    def stop(self):
        self.stopRequest = True
        self.serhan.stop()

    def callback(self, message):
        if message[0] == Messenger.COMPONENT_MESSAGE:
            self.processComponentMessage(message)
        elif message[0] == Messenger.AP_TEXT_LOG:
            self.processTextLogMessage(message)
        elif message[0] == Messenger.MEDIA_PASSPORT:
            self.processMediaPassport(message)
        elif message[0] == Messenger.COMPONENT_COUNT:
            self.processComponentCount(message)

        if self.comparator is not None:
            status = self.comparator(message)
            if status[0] and not status[1]:
                self.printError(status[2]["result"])
            if status[1]:
                self.event = status

    def printError(self, code):
        codeMap = {
                0: "SUCCESS",
                1: "COMPONENT_NOT_FOUND",
                2: "FIELD_NOT_FOUND",
                3: "FIELD_TYPE_MISMATCH",
                4: "FIELD_RANGE_ERROR",
                5: "FIELD_ERROR",
                6: "FIELD_READ_ONLY",
                7: "FILE_NOT_FOUND",
                8: "FILE_ERROR",
                9: "FIELD_UNAVAILABLE",
                10: "FIELD_TIMEOUT",
                11: "FILE_ACCESS_ERROR",
                12: "FILE_POSITION_ERROR",
                13: "FILE_TIMEOUT",
                14: "COMMAND_FAILED",
                15: "COMMAND_UNSUPPORTED"
        }
        print("Error: 0x%02X (%s)" % (code, codeMap[code]))

    def await(self, comparator):
        self.comparator = None
        self.event = (False, False)
        self.comparator = comparator

    def wait(self, comparator, action=None, maxRetries=16):
        TIMEOUT = 0.2

        retries = maxRetries
        self.await(comparator)
        while retries > 0:
            if action is not None:
                action()
            startTime = time.time()
            while not self.event[0]:
                if time.time() - startTime >= TIMEOUT:
                    break
                time.sleep(0.001)
            if self.event[0]:
                break
            retries -= 1
        return self.event[1]

    def parseFileDescription(self, message):
        msgComponent = ord(message[1][0])
        msgFile = ord(message[1][1])
        msgStatus = ord(message[1][2])
        msgSize = struct.unpack("<I", message[1][3:7])[0]
        msgChecksum = struct.unpack("<I", message[1][7:11])[0]
        return {"component": msgComponent, "file": msgFile, "status": msgStatus, "size": msgSize,\
                "checksum": msgChecksum}

    def parseFileInfoError(self, message):
        msgComponent = ord(message[1][0])
        msgFile = ord(message[1][1])
        msgResult = ord(message[1][2])
        return {"component": msgComponent, "file": msgFile, "result": msgResult}

    def waitForFileDescription(self, message):
        if message[0] == Messenger.COMPONENT_FILE_INFO:
            parsed = self.parseFileDescription(message)
            debug("File info received: component %u, file %u, status %u, size %u, checksum %08X" %\
                  (parsed["component"], parsed["file"], parsed["status"], parsed["size"], parsed["checksum"]))
            return (True, True, parsed)
        elif message[1] == Messenger.COMPONENT_FILE_INFO_ERROR:
            parsed = self.parseFileInfoError(message)
            debug("File info error: component %u, file %u, result %u" %\
                  (parsed["component"], parsed["file"], parsed["result"]))
            return (True, False, parsed)
        else:
            return (False, False, {})

    def requestFileDescription(self, component, identifier, full=True):
        flags = 0x03 if full else 0x00
        message = (Messenger.COMPONENT_FILE_DESCRIBE, "%c%c%c" % (chr(component), chr(identifier), chr(flags)))
        debug("Request file description: component %u, file %u" % (component, identifier))
        self.serhan.send(message)

    def parseFileReadResponse(self, message):
        msgComponent = ord(message[1][0])
        msgFile = ord(message[1][1])
        msgPosition = struct.unpack("<I", message[1][2:6])[0]
        msgData = message[1][6:]
        return {"component": msgComponent, "file": msgFile, "position": msgPosition, "data": msgData}

    def parseFileReadError(self, message):
        msgComponent = ord(message[1][0])
        msgFile = ord(message[1][1])
        msgPosition = struct.unpack("<I", message[1][2:6])[0]
        msgResult = ord(message[1][6])
        return {"component": msgComponent, "file": msgFile, "position": msgPosition, "result": msgResult}

    def waitForFileReadResponse(self, message):
        if message[0] == Messenger.COMPONENT_FILE_READ_RESPONSE:
            parsed = self.parseFileReadResponse(message)
            debug("File read response received: component %u, file %u, position %u" %\
                  (parsed["component"], parsed["file"], parsed["position"]))
            return (True, True, parsed)
        elif message[0] == Messenger.COMPONENT_FILE_READ_ERROR:
            parsed = self.parseFileReadError(message)
            return (True, False, parsed)
        else:
            return (False, False, {})

    def readFileChunk(self, component, identifier, position, size, fragment):
        positionStr = struct.pack("<I", position)
        lengthStr = struct.pack("<H", size)
        message = (Messenger.COMPONENT_FILE_READ, "%c%c%s%s%c" %\
                (chr(component), chr(identifier), positionStr, lengthStr, chr(fragment)))
        debug("Read file chunk: component %u, file %u, position %u, size %u, fragment %u" %\
                (component, identifier, position, size, fragment))
        self.serhan.send(message)

    def parseFileWriteResponse(self, message):
        msgComponent = ord(message[1][0])
        msgFile = ord(message[1][1])
        msgPosition = struct.unpack("<I", message[1][2:6])[0]
        msgResult = ord(message[1][6])
        return {"component": msgComponent, "file": msgFile, "position": msgPosition, "result": msgResult}

    def waitForFileWriteResponse(self, message):
        if message[0] == Messenger.COMPONENT_FILE_WRITE_RESPONSE:
            parsed = self.parseFileWriteResponse(message)
            debug("File write response received: component %u, file %u, result %u, position %u" %\
                  (parsed["component"], parsed["file"], parsed["result"], parsed["position"]))
            return (True, True if parsed["result"] == 0 else False, parsed)
        else:
            return (False, False, {})

    def writeFileChunk(self, component, identifier, position, chunk):
        positionStr = struct.pack("<I", position)
        message = (Messenger.COMPONENT_FILE_WRITE, "%c%c%s%s" %\
                (chr(component), chr(identifier), positionStr, chunk))
        debug("Write file chunk: component %u, file %u, position %u, size %u" %\
              (component, identifier, position, len(chunk)))
        self.serhan.send(message)

    def parseComponentCount(self, message):
        msgCount = ord(message[1][0])
        return {"count": msgCount}

    def waitForComponentCount(self, message):
        if message[0] == Messenger.COMPONENT_COUNT:
            parsed = self.parseComponentCount(message)
            debug("Component count received: count %u" % parsed["count"])
            return (True, True, parsed)
        else:
            return (False, False, {})

    def requestComponentCount(self):
        message = (Messenger.COMPONENT_COUNT_REQUEST, "")
        debug("Request component count")
        self.serhan.send(message)

    def parseComponentInfo(self, message):
        msgComponent = ord(message[1][0])

        msgType = ord(message[1][1])
        msgSize = ord(message[1][2])

        # Try to parse as 1.3 message at first
        msgNameSize = ord(message[1][3])
        if len(message[1]) == 4 + msgNameSize:
            msgMinor, msgRevision, msgHash = 0, 0, 0
            msgName = message[1][4:4 + msgNameSize]
        else:
            msgMinor = ord(message[1][3])
            msgRevision = struct.unpack("<H", message[1][4:6])[0]
            msgHash = struct.unpack("<I", message[1][6:10])[0]
            msgNameSize = ord(message[1][10])
            msgName = message[1][11:11 + msgNameSize]

        return {"component": msgComponent, "type": msgType, "size": msgSize, "minor": msgMinor,
                "revision": msgRevision, "hash": msgHash, "name": msgName}

    def parseComponentInfoError(self, message):
        msgComponent = ord(message[1][0])
        msgResult = ord(message[1][1])
        return {"component": msgComponent, "result": msgResult}

    def waitForComponentInfo(self, message):
        if message[0] == Messenger.COMPONENT_INFO:
            parsed = self.parseComponentInfo(message)
            debug("Component info received: component %u, version %u.%u.%u, hash %08X, name \"%s\", count %u, " %\
                    (parsed["component"], parsed["type"], parsed["minor"], parsed["revision"], parsed["hash"],\
                    parsed["name"], parsed["size"]))
            return (True, True, parsed)
        elif message[0] == Messenger.COMPONENT_INFO_ERROR:
            parsed = self.parseComponentInfoError(message)
            return (True, False, parsed)
        else:
            return (False, False, {})

    def requestComponentInfo(self, component):
        message = (Messenger.COMPONENT_INFO_REQUEST, "%c" % chr(component))
        debug("Request component %u info" % component)
        self.serhan.send(message)

    def parseFieldInfo(self, message):
        position = 0
        msgComponent = ord(message[1][position])
        position += 1
        msgField = ord(message[1][position])
        position += 1
        msgType = ord(message[1][position])
        position += 1
        msgNameSize = ord(message[1][position])
        position += 1
        msgName = message[1][position:position + msgNameSize]
        position += msgNameSize

        if len(message[1]) > position:
            msgScale = struct.unpack("b", message[1][position])[0]
            position += 1

            fieldSize = Messenger.typeToSize(msgType)

            msgUnitSize = ord(message[1][position])
            position += 1
            msgUnit = message[1][position:position + msgUnitSize]
            position += msgUnitSize

            msgMinValue = Messenger.fetchValue(msgType, message[1][position:position + fieldSize])
            position += fieldSize
            msgMaxValue = Messenger.fetchValue(msgType, message[1][position:position + fieldSize])
        else:
            msgScale = 0
            msgUnit = ""
            msgMinValue = None
            msgMaxValue = None

        return {"component": msgComponent, "field": msgField, "type": msgType, "name": msgName,
                "scale": msgScale, "unit": msgUnit, "min": msgMinValue, "max": msgMaxValue}

    def parseFieldError(self, message):
        msgComponent = ord(message[1][0])
        msgField = ord(message[1][1])
        msgResult = ord(message[1][2])
        return {"component": msgComponent, "field": msgField, "result": msgResult}

    def waitForFieldInfo(self, message):
        if message[0] == Messenger.COMPONENT_FIELD_INFO:
            parsed = self.parseFieldInfo(message)
            debug("Component %u field %u info: type %u, name \"%s\", scale %i, unit \"%s\", min %s, max %s" %\
                  (parsed["component"], parsed["field"], parsed["type"], parsed["name"], parsed["scale"],
                   parsed["unit"], str(parsed["min"]), str(parsed["max"])))
            return (True, True, parsed)
        elif message[0] == Messenger.COMPONENT_FIELD_ERROR:
            parsed = self.parseFieldError(message)
            return (True, False, parsed)
        else:
            return (False, False, {})

    def requestFieldInfo(self, component, field):
        message = (Messenger.COMPONENT_FIELD_DESCRIBE, "%c%c" % (chr(component), chr(field))) 
        debug("Request component %u, field %u info" % (component, field))
        self.serhan.send(message)

    def readFile(self, index, fd, part=0):
        WINDOW_SIZE, CHUNK_SIZE = self.fragmentation
        RX_TIMEOUT = 0.25

        def descriptionHook(obj, message):
            res = obj.waitForFileDescription(message)
            if res[1]:
                parsed = self.parseFileDescription(message)
                obj.fileReader.size = parsed["size"]
                obj.fileReader.checksum = parsed["checksum"]
                print("File size %u" % parsed["size"])
            return res
        descriptionHookHandler = lambda message: descriptionHook(self, message)

        def readResponseHook(obj, message):
            res = obj.waitForFileReadResponse(message)
            if res[1]:
                parsed = self.parseFileReadResponse(message)
                if obj.fileReader.position == parsed["position"]:
                    obj.fileReader.position = parsed["position"] + len(parsed["data"])
                    obj.fileReader.data += parsed["data"]
                    obj.txTime = time.time()
                    debug("Chunk [%06X:%06X] appended" % (parsed["position"], parsed["position"] + len(parsed["data"])))
                else:
                    debug("Chunk [%06X:%06X] dropped" % (parsed["position"], parsed["position"] + len(parsed["data"])))
            return res
        readResponseHookHandler = lambda message: readResponseHook(self, message)

        class FileReader:
            def __init__(self):
                self.data = ""
                self.position = 0
                self.size = 0
                self.checksum = 0
                self.rxTime = time.time()

        self.fileReader = FileReader()

        infoTime = time.time()

        # Get description
        if not self.wait(descriptionHookHandler, lambda: self.requestFileDescription(index, fd, True)):
            print("Failed to read file description")
            return None

        startTime = time.time()

        position = 0
        self.await(readResponseHookHandler)
        while not self.stopRequest:
            if position < self.fileReader.size and position - self.fileReader.position < WINDOW_SIZE / 2:
                delta = self.fileReader.size - position
                nextChunkSize = WINDOW_SIZE if delta > WINDOW_SIZE else delta
                self.readFileChunk(index, fd, position, nextChunkSize, CHUNK_SIZE)
                position += nextChunkSize

            if self.fileReader.position == self.fileReader.size:
                break
            if part != 0 and self.fileReader.position >= part:
                break
            if time.time() - self.fileReader.rxTime >= RX_TIMEOUT:
                debug("Data timeout: requested %u, received %u" % (position, self.fileReader.position))
                position = self.fileReader.position
                self.fileReader.rxTime = time.time()
            time.sleep(0.001)

        passed = time.time() - startTime

        crc = binascii.crc32(self.fileReader.data) & 0xFFFFFFFF
        print("Checksum for received data %08X, expected %08X" % (crc, self.fileReader.checksum))
        print("Transfer finished in %.3f s, info in %.3f s, received %u, rate %u bit/s" %\
              (passed, (startTime - infoTime), position, int(position * 8 / passed)))

        return self.fileReader.data

    def writeFile(self, index, fd, payload, append):
        WINDOW_SIZE, CHUNK_SIZE = self.fragmentation
        RX_TIMEOUT = 0.25

        class FileSender:
            def __init__(self, length):
                self.rxPos = 0
                self.rxTime = time.time()
                self.retries = 0
                self.total = length
                self.size = 0

        # Get description
#         if not self.wait(self.waitForFileDescription, lambda: self.requestFileDescription(index, fd)):
#             return

        self.fileSender = FileSender(len(payload))
        txPos = 0

        def descriptionHook(obj, message):
            res = obj.waitForFileDescription(message)
            if res[1]:
                parsed = self.parseFileDescription(message)
                obj.fileSender.size = parsed["size"]
                print("File size %u" % parsed["size"])
            return res
        descriptionHookHandler = lambda message: descriptionHook(self, message)

        # Start transaction
        def writeResponseHook(obj, message):
            res = obj.waitForFileWriteResponse(message)
            if res[1]:
                parsed = self.parseFileWriteResponse(message)
                if obj.fileSender.total - parsed["position"] >= CHUNK_SIZE:
                    expectedLength = CHUNK_SIZE
                else:
                    expectedLength = obj.fileSender.total - parsed["position"]
                obj.fileSender.rxPos = parsed["position"] + expectedLength
                obj.fileSender.rxTime = time.time()
                obj.fileSender.retries = 0
#                 debug("Committed position updated to %u" % self.fileSender.rxPos)
            return res
        writeResponseHookHandler = lambda message: writeResponseHook(self, message)

        startTime = time.time()
        offset = 0

        if not append:
            # Restart file
            if not self.wait(self.waitForFileWriteResponse, lambda: self.writeFileChunk(index, fd, 0, "")):
                print("Failed to start file transaction")
                return
        else:
            # Get description
            if not self.wait(descriptionHookHandler, lambda: self.requestFileDescription(index, fd, True)):
                print("Failed to read file description")
                return None
            offset = self.fileSender.size
            self.fileSender.rxPos = offset
            txPos = offset
            print("Continue writing from %u" % offset)

        self.await(writeResponseHookHandler)
        while not self.stopRequest:
            while txPos - offset < len(payload) and txPos - self.fileSender.rxPos < WINDOW_SIZE:
                if len(payload) - (txPos - offset) >= CHUNK_SIZE:
                    currentChunkSize = CHUNK_SIZE
                else:
                    currentChunkSize = len(payload) - (txPos - offset)
                chunk = payload[txPos - offset:txPos - offset + currentChunkSize]
                self.writeFileChunk(index, fd, txPos, chunk)
                txPos += currentChunkSize

            # Transfer finished
            if txPos - offset == len(payload) and self.fileSender.rxPos == txPos:
                debug("Payload transfer finished, sent %u" % (txPos - offset))
                break

            # Data timeout occurred
            if time.time() - self.fileSender.rxTime >= RX_TIMEOUT:
                debug("Data timeout: sent %u, accepted %u" % ((txPos - offset), (self.fileSender.rxPos - offset)))
                txPos = self.fileSender.rxPos
                self.fileSender.rxTime = time.time()
                self.fileSender.retries += 1

            if self.fileSender.retries > 8:
                print("Failed to write file chunk")
                return

            time.sleep(0.001)

        # Finalize transaction
        if not self.wait(self.waitForFileWriteResponse, lambda: self.writeFileChunk(index, fd, txPos, "")):
            print("Failed to finalize file transaction")
            return

        passed = time.time() - startTime
        print("Transfer finished in %.3f s, sent %u, rate %u bit/s" % (passed, txPos - offset,\
                int((txPos - offset) * 8 / passed)))

    def lookupComponents(self):
        self.hub = Messenger.Hub()

        def componentCountHook(obj, message):
            res = obj.waitForComponentCount(message)
            if res[1]:
                parsed = self.parseComponentCount(message)
                self.hub.mutex.acquire()
                obj.hub.componentCount = parsed["count"]
                self.hub.mutex.release()
            return res
        componentCountHookFunc = lambda message: componentCountHook(self, message)

        def componentInfoHook(obj, message):
            res = obj.waitForComponentInfo(message)
            if res[1]:
                parsed = self.parseComponentInfo(message)
                obj.hub.mutex.acquire()
                if parsed["component"] not in obj.hub.components.keys():
                    obj.hub.components[parsed["component"]] = Messenger.Component(parsed["component"],\
                            parsed["type"], parsed["minor"], parsed["revision"], parsed["hash"],\
                            parsed["name"], parsed["size"])
                obj.hub.mutex.release()
            return res
        componentInfoHookFunc = lambda message: componentInfoHook(self, message)

        def componentFieldInfoHook(obj, message, field):
            res = obj.waitForFieldInfo(message)
            if res[1]:
                parsed = self.parseFieldInfo(message)

                if parsed["field"] != field:
                    return (False, False, {})

                obj.hub.mutex.acquire()
                if parsed["component"] in obj.hub.components.keys():
                    if parsed["field"] not in obj.hub.components[parsed["component"]].fields.keys():
                        obj.hub.components[parsed["component"]].fields[parsed["field"]] =\
                                Messenger.ComponentField(parsed["field"], parsed["type"], parsed["name"],
                                        parsed["scale"], parsed["unit"], parsed["min"], parsed["max"])
                obj.hub.mutex.release()
            return res

        if not self.wait(componentCountHookFunc, lambda: self.requestComponentCount()):
            print("Component count request failed")
            return

        self.hub.mutex.acquire()
        iCount = self.hub.componentCount
        print("Components found: %u" % iCount)
        self.hub.mutex.release()

        for i in range(0, iCount):
            if not self.wait(componentInfoHookFunc, lambda: self.requestComponentInfo(i)):
                print("Component %u info request failed" % i)
                return

            self.hub.mutex.acquire()
            jCount = self.hub.components[i].fieldCount
            print("Component %u: name \"%s\", version %u.%u.%u, hash %08X, count %u" %\
                    (i, self.hub.components[i].name, self.hub.components[i].type, self.hub.components[i].minor,\
                    self.hub.components[i].revision, self.hub.components[i].hash, jCount))
            self.hub.mutex.release()

            for j in range(0, jCount):
                componentFieldInfoHookFunc = lambda message: componentFieldInfoHook(self, message, j)
                if not self.wait(componentFieldInfoHookFunc, lambda: self.requestFieldInfo(i, j)):
                    debug("\tField %u info request failed" % j)
                    return

                self.hub.mutex.acquire()
                print("\tField % 3u: name \"%s\", type % 2s, scale %i, unit \"%s\", range [%s:%s]"
                        % (j, self.hub.components[i].fields[j].name,
                                Messenger.typeToString(self.hub.components[i].fields[j].type),
                                self.hub.components[i].fields[j].scale, self.hub.components[i].fields[j].unit,
                                str(self.hub.components[i].fields[j].minimal),
                                str(self.hub.components[i].fields[j].maximal)))
                self.hub.mutex.release()

            # Look for file streams
            for j in range(0, 256):
                if not self.wait(self.waitForFileDescription, lambda: self.requestFileDescription(i, j, False), 1):
                    break
                else:
                    print("\tFile stream %u found" % j)

    def parseFieldResponse(self, message):
        msgComponent = ord(message[1][0])
        msgField = ord(message[1][1])
        msgResult = ord(message[1][2])
        return {"component": msgComponent, "field": msgField, "result": msgResult}

    def waitForFieldResponse(self, message):
        if message[0] == Messenger.COMPONENT_FIELD_RESPONSE:
            parsed = self.parseFieldResponse(message)
            debug("Field response received: component %u, field %u, result %u" %\
                  (parsed["component"], parsed["field"], parsed["result"]))
            return (True, True if parsed["result"] == 0 else False, parsed)
        else:
            return (False, False, {})

    def setField(self, index, field, value):
        self.hub.mutex.acquire()
        if index not in self.hub.components.keys() or field not in self.hub.components[index].fields.keys():
            self.hub.mutex.release()
            return None

        def fieldResponseHook(obj, message):
            res = obj.waitForFieldResponse(message)
            return res
        fieldResponseHookHandler = lambda message: fieldResponseHook(self, message)

        print("Write component %u, field %u, value %s" % (index, field, value))

        def req(index, field):
            desc = self.hub.components[index].fields[field]
            payload = "%c%c%c%s" % (chr(index), chr(field), chr(desc.type), Messenger.createValue(desc.type, value))
            message = (Messenger.COMPONENT_FIELD, payload)
            self.serhan.send(message)

        startTime = time.time()
        if not self.wait(fieldResponseHookHandler, lambda: req(index, field)):
            print("Failed to write value")
            self.hub.mutex.release()
            return None
        endTime = time.time()
        print("Passed %.3f " % (endTime - startTime))

        self.hub.mutex.release()
        return endTime - startTime

    def parseFieldValue(self, message):
        msgComponent = ord(message[1][0])
        msgFields = [ord(message[1][1])]
        msgType = ord(message[1][2])
        msgValues = [Messenger.fetchValue(msgType, message[1][3:])]
        return {"component": msgComponent, "fields": msgFields, "values": msgValues}

    def parseMultiFieldValue(self, message):
        msgComponent = ord(message[1][0])
        component = self.hub.components[msgComponent]
        i = 2 # Skip component identifier and flags
        msgValues = []
        msgFields = []
        while i < len(message[1]):
            fieldId = ord(message[1][i])
            msgType = component.fields[fieldId].type
            msgFields += [fieldId]
            msgValues += [Messenger.fetchValue(msgType, message[1][i + 1:])]
            i += 1 + Messenger.typeToSize(msgType)
        return {"component": msgComponent, "fields": msgFields, "values": msgValues}

    def waitForFieldValue(self, message):
        if message[0] == Messenger.COMPONENT_FIELD:
            parsed = self.parseFieldValue(message)
            debug("Field value received: component %u, fields [%s], values [%s]" % (parsed["component"],\
                    ", ".join(map(str, parsed["fields"])),\
                    ", ".join(map(str, parsed["values"]))))
            return (True, True, parsed)
        elif message[0] == Messenger.COMPONENT_MULTI_FIELD:
            parsed = self.parseMultiFieldValue(message)
            debug("Field value received: component %u, fields [%s], values [%s]" % (parsed["component"],\
                    ", ".join(map(str, parsed["fields"])),\
                    ", ".join(map(str, parsed["values"]))))
            return (True, True, parsed)
        else:
            return (False, False, {})

    def requestField(self, index, fields):
        if len(fields) == 1:
            payload = chr(index) + chr(fields[0])
            message = (Messenger.COMPONENT_FIELD_REQUEST, payload)
        else:
            payload = chr(index)
            for e in fields:
                payload += chr(e)
            message = (Messenger.COMPONENT_MULTI_FIELD_REQUEST, payload)
        self.serhan.send(message)

    def getField(self, index, fields):
        self.hub.mutex.acquire()
        if index not in self.hub.components.keys():
            self.hub.mutex.release()
            return None
        for e in fields:
            if e not in self.hub.components[index].fields.keys():
                self.hub.mutex.release()
                return None

        class ValueContainer:
            def __init__(self):
                self.values = None

        self.valueContainer = ValueContainer()

        def fieldValueHook(obj, message):
            res = obj.waitForFieldValue(message)
            if res[1]:
                if message[0] == Messenger.COMPONENT_FIELD:
                    parsed = self.parseFieldValue(message)
                else:
                    parsed = self.parseMultiFieldValue(message)
                obj.valueContainer.values = parsed["values"]
            return res
        fieldValueHookHandler = lambda message: fieldValueHook(self, message)

        startTime = time.time()
        if not self.wait(fieldValueHookHandler, lambda: self.requestField(index, fields)):
            print("Failed to read fields value")
            self.hub.mutex.release()
            return None
        endTime = time.time()
        print("Passed %.3f, read [%s]" % (endTime - startTime, ", ".join(map(str, self.valueContainer.values))))

        self.hub.mutex.release()
        return self.valueContainer.values

    def parsePassport(self, message):
        msgNumber = struct.unpack("<H", message[1][0:2])[0]
        msgLatitude = struct.unpack("<i", message[1][2:6])[0]
        msgLongitude = struct.unpack("<i", message[1][6:10])[0]
        msgAltitude = struct.unpack("<h", message[1][10:12])[0]
        msgRoll = struct.unpack("<h", message[1][12:14])[0]
        msgPitch = struct.unpack("<h", message[1][14:16])[0]
        msgYaw = struct.unpack("<h", message[1][16:18])[0]
        return {"number": msgNumber, "latitude": msgLatitude, "longitude": msgLongitude, "altitude": msgAltitude,
                "roll": msgRoll, "pitch": msgPitch, "yaw": msgYaw}

    def waitForPassport(self, message):
        if message[0] == Messenger.MEDIA_PASSPORT:
            parsed = self.processMediaPassport(message)
            print(parsed)
            return (True, True, parsed)
        else:
            return (False, False, {})

    def requestPassport(self, number):
        payload = struct.pack("<H", number)
        message = (Messenger.REQ_PASS, payload)
        self.serhan.send(message)

    def getPassport(self, number):
        def passportHook(obj, message):
            return obj.waitForPassport(message)
        passportHookHandler = lambda message: passportHook(self, message)

        startTime = time.time()
        acquired = self.wait(passportHookHandler, lambda: self.requestPassport(number))
        endTime = time.time()

        if acquired:
            print("Passport read in %.3f" % (endTime - startTime))
            return True
        else:
            print("Failed to read passport %u" % number)
            return False

    def getPassportStorage(self, count):
        for i in range(0, count):
            if not self.getPassport(i):
                break;

    def setMode(self, mode):
        payload = "%c" % chr(mode)
        message = (Messenger.SET_MODE, payload)
        self.serhan.send(message)

    def halt(self):
        payload = "\x00"
        message = (Messenger.BOOT, payload)
        self.serhan.send(message)

    def boot(self):
        def bootRequest():
            self.serhan.sendRaw("\x55\xAA\x00\x06\x00\x00\x00\x00")
            self.serhan.sendRaw("\x55\xAA\x00\x01\x00\x00\x00\x00")

        self.halt()
        time.sleep(0.01)
        bootRequest()
        time.sleep(0.01)

    def format(self):
        payload = "\x0E"
        message = (Messenger.SYSTEM_COMMAND, payload)
        self.serhan.send(message)

    def dec(self):
        payload = "\x0D"
        message = (Messenger.SYSTEM_COMMAND, payload)
        self.serhan.send(message)


def showGnssInfo(data):
    GNSS_SYSTEMS = {0: "GPS", 1: "SBAS", 2: "GALILEO", 3: "BEIDOU", 4: "IMES", 5: "QZSS", 6: "GLONASS"}
    SNR = (0.0, 60.0)
    WIDTH = 52.0

    count = len(data) / 6
    satellites = []
    for i in range(0, count):
        strength = struct.unpack("B", data[i * 6 + 0:i * 6 + 1])[0]
        elevation = struct.unpack("b", data[i * 6 + 1:i * 6 + 2])[0]
        azimuth = struct.unpack("<h", data[i * 6 + 2:i * 6 + 4])[0]
        number = struct.unpack("<H", data[i * 6 + 4:i * 6 + 6])[0]
        sysId, satId = ((number >> 8) & 0xFF), (number & 0xFF)
        satellites.append((sysId, satId, strength, elevation, azimuth))
        debug("%u.%u: SNR %u dB, elevation %i deg, azimuth %i deg" % satellites[-1])
    print("System  ID  El   Az   SNR")
    for e in satellites:
        if e[0] in GNSS_SYSTEMS.keys():
            level = int(float(e[2]) * (WIDTH / (SNR[1] - SNR[0])))
            output = ""
            output += GNSS_SYSTEMS[e[0]] + " " * (8 - len(GNSS_SYSTEMS[e[0]]))
            output += "%3u " % e[1]
            output += "%+4i " % e[3]
            output += "%+4i " % e[4]
            output += "%2u [" % e[2]
            output += "#" * level + " " * int(WIDTH - level) + "]"
            print(output)

args = argparse.ArgumentParser()
args.add_argument("--serial", dest="serial", help="serial port name", default="")
args.add_argument("--address", dest="ip", help="server address", default="")
args.add_argument("--modem", dest="modem", help="modem socket", default="1:2")
args.add_argument("--read", dest="readAddress", help="stream to read", default="-1:-1")
args.add_argument("--write", dest="writeAddress", help="stream to write", default="-1:-1")
args.add_argument("--runScript", dest="run", help="runs the loaded lua script", default=False, action="store_true")
args.add_argument("--part", dest="part", help="loaded only a part of file", default=0)
args.add_argument("--append", dest="append", help="append to the end of stream", default=False, action="store_true")
args.add_argument("--fragment", dest="fragment", help="fragment size", default=40)
args.add_argument("--chunk", dest="chunk", help="total chunk size", default=320)
args.add_argument("--gnss", dest="gnss", help="render file as GNSS information", default=False, action="store_true")
args.add_argument("--filter", dest="deviceFilter", help="filter message by device number", default="")
args.add_argument("-f", dest="localFile", help="local file to read or write", default="")
args.add_argument("-d", dest="debug", help="enable debug messages", default=False, action="store_true")
args.add_argument("-v", dest="verbose", help="enable verbose mode", default=False, action="store_true")
options = args.parse_args()

ports = {}
if options.ip != "":
    ports["ip"] = options.ip
    ports["modem"] = options.modem
if options.serial != "":
    ports["serial"] = options.serial
console = True
debugEnabled = options.debug
verboseEnabled = options.verbose

quiet = options.localFile != "" or options.writeAddress != "-1:-1" or options.readAddress != "-1:-1"
deviceFilter = []
if not quiet:
    for entry in filter(lambda x: len(x) > 0, options.deviceFilter.split(",")):
        try:
            id = int(entry)
            deviceFilter.append(id)
        except:
            print("Incorrect device filter: %s" % entry)
            exit()

msgr = Messenger(ports, deviceFilter)
msgr.fragmentation = (int(options.chunk), int(options.fragment))
listen = True

msgr.lookupComponents()
if options.writeAddress != "-1:-1" and options.localFile != "":
    address = None
    parts = options.writeAddress.split(":")
    try:
        address = map(int, parts)
    except:
        items = msgr.hub.components.values()
        if parts[0] in map(lambda x: x.name, items):
            component = filter(lambda x: x.name == parts[0], items)[0]
            try:
                address = (component.index, int(parts[1]))
            except:
                print("Error processing arguments")
        else:
            print("No such component")
    if address is not None:
        payload = open(options.localFile, "rb").read()
        msgr.writeFile(address[0], address[1], payload, options.append)
    console = False

if options.readAddress != "-1:-1" and options.localFile != "":
    address = None
    parts = options.readAddress.split(":")
    try:
        address = map(int, parts)
    except:
        items = msgr.hub.components.values()
        if parts[0] in map(lambda x: x.name, items):
            component = filter(lambda x: x.name == parts[0], items)[0]
            try:
                address = (component.index, int(parts[1]))
            except:
                print("Error processing arguments")
        else:
            print("No such component")
    if address is not None:
        payload = msgr.readFile(address[0], address[1], int(options.part))
        if payload is not None:
            outputFile = open(options.localFile, "wb")
            outputFile.write(payload)
            outputFile.close()
    console = False

if options.run == True:    
    msgr.setField(9, 0, 1)
    time.sleep(2.0)
    console = False

if console:
    def terminateHandler(signum, frame):
        global termRequest
        print("Termination requested")
        termRequest = True

    termRequest = False
    signal.signal(signal.SIGINT, terminateHandler)

    while not termRequest:
        userInput = raw_input("> ")
        userInput = userInput.replace("\r", "").replace("\n", "")
        chunks = filter(lambda x: len(x) > 0, userInput.split(" "))

        if len(chunks) == 0:
            continue
        elif chunks[0] == "exit":
            break
        elif chunks[0] == "write":
            if len(chunks) != 3:
                print("Incorrect argument count")
                continue
            address = chunks[1].split(":")
            if len(address) != 2:
                print("Incorrect address: %s" % chunks[1])
                continue
            try:
                payload = open(chunks[2], "rb").read()
                msgr.writeFile(int(address[0]), int(address[1]), payload, False)
            except:
                print("Wrong arguments")
        elif chunks[0] == "set":
            if len(chunks) < 3:
                print("Incorrect argument count")
                continue
            address = chunks[1].split(":")
            if len(address) != 2:
                print("Incorrect address: %s" % chunks[1])
                continue
            try:
                cnt = int(chunks[3]) if len(chunks) >= 4 else 1
                for i in range(0, cnt):
                    component, field = int(address[0]), int(address[1])
                    value = chunks[2]
                    msgr.setField(component, field, value)
                    time.sleep(2.0)
            except:
                print("Wrong arguments")
        elif chunks[0] == "get":
            if len(chunks) < 2:
                print("Incorrect argument count")
            address = chunks[1].split(":")
            if len(address) != 2:
                print("Incorrect address: %s" % chunks[1])
                continue
            try:
                cnt = int(chunks[2]) if len(chunks) >= 3 else 1
                for i in range(0, cnt):
                    component, fields = int(address[0]), map(int, address[1].split(","))
                    msgr.getField(component, fields)
                    time.sleep(2.0)
            except:
                print("Wrong arguments")
        elif chunks[0] == "start":
            msgr.setMode(2)
        elif chunks[0] == "halt":
            msgr.halt()
        elif chunks[0] == "boot":
            msgr.boot()
        elif chunks[0] == "format":
            msgr.format()
        elif chunks[0] == "dec":
            msgr.dec()
        elif chunks[0] == "pass":
            if len(chunks) != 2:
                print("Incorrect argument count")
                continue
            try:
                count = int(chunks[1])
                msgr.getPassportStorage(count)
            except:
                pass
        elif chunks[0] == "gnss":
            if len(chunks) != 3:
                print("Incorrect argument count")
                continue
            address = chunks[1].split(":")
            if len(address) != 2:
                print("Incorrect address: %s" % chunks[1])
                continue
            try:
                component, file, measure = int(address[0]), int(address[1]), int(chunks[2])
                msgr.setField(component, measure, True)
                measureStart = time.time()
                measured = False
                while time.time() < measureStart + 5.0:
                    res = msgr.getField(component, [measure])
                    if res is not None and res[0] == False:
                        measured = True
                        break
                    time.sleep(0.25)
                if measured:
                    payload = msgr.readFile(component, file)
                    if payload is not None:
                        showGnssInfo(payload)
            except:
                print("Wrong arguments")

msgr.stop()
