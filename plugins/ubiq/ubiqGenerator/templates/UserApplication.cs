using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;
using System.IO;
using Ubiq;
using Ubiq.TcpIpAPI;
using Ubiq.MessagingAPI;
using Ubiq.QRealAPI;

namespace DeviceService
{
    /* Структура Message должна быть в том же namespace   */

    public class UserApplication : QRealApp
    {
        TState state;
        int deviceID;            // Что с этим делать????
        int currentInterval;     // И с этим ???

        const string KBoxName = "DeviceServicesMailBox";

        public override void MainOverride()
        {
            UseMessageInput(DefaultMailBox);
            UseKbdInput();

            state = TState.EStateGettingDeviceID;

            ProcessIncomingMessages();
        }

        public override void OnMailBoxMessage(Message msg)
        {
            switch (state)
            {
                case TState.EStateGettingDeviceID:
                    {
                        state = TState.EStateInitTransfer;
                        deviceID = GetDeviceID();
                    }
                    break;

                case TState.EStateSwitchDevice:
                    {
                        state = TState.EStateInitTransfer;
                        SendMessageToDispatcher(deviceID, Message.KMsgSuspendTransfer);
                        deviceID = GetDeviceID();
                    }
                    break;
                case TState.EStateInitTransfer:
                    {
                        ShowInfo(Message.KMsgDevRequest);

                        switch (RequestDevice(deviceID))
                        {
                            case Message.KErrNo:
                                {
                                    if (MainMenu() != NewDevice)
                                    {
                                        state = TState.EStateTransferActive;
                                        SetDeviceParameters();
                                    }
                                    else
                                        state = TState.EStateGettingDeviceID;
                                }
                                break;
                            case Message.KErrDeviceNotRegistered:
                                {
                                    state = TState.EStateGettingDeviceID;
                                    ShowError(Message.KErrDeviceNotRegistered);
                                }
                                break;
                            case Message.KErrDeviceNotReady:
                                {
                                    state = TState.EStateInitTransfer;
                                    ShowError(Message.KErrDeviceNotReady);
                                }
                            default:
                                throw new ArgumentException("Bad message code");
                        }
                    }
                    break;
                case TState.EStateTransferActive:
                    {
                        switch (msg.Command)
                        {
                            case Message.KMsgReceivedData:
                                {
                                    ProcessIncomingData(msg);
                                }
                                break;
                            case Message.KMsgSetParameters:
                                {
                                    SetDeviceParameters();   // ??????
                                }
                                break;
                            case Message.KMsgSuspendTransfer:
                                {
                                    state = TState.EStateTransferSuspended;
                                    ShowInfo(Message.KMsgSuspendTransfer);
                                }
                                break;
                            case Message.KMsgResumeTransfer:
                                {
                                    state = TState.EStateResumeTransfer;
                                }
                                break;
                            case Message.KMsgStopTransfer:
                                {
                                    state = TState.EStateResumeTransfer;
                                    ShowInfo(Message.KMsgStopTransfer);
                                }
                                break;
                            case Message.KMsgError:
                                {
                                    switch (msg.Code)
                                    {
                                        case Message.KErrDeviceNotReady:
                                        case Message.KErrLostConnection:
                                            {
                                                state = TState.EStateResumeTransfer;
                                                ShowError(msg.Code);
                                            }
                                            break;
                                        default:
                                            {
                                                throw new ArgumentException("Bad error code");
                                            }
                                    }
                                }
                                break;
                        }
                    }
                    break;
                case TState.EStateTransferSuspended:
                    {
                        if (msg.Command == Message.KMsgResumeTransfer)
                        {
                            state = TState.EStateResumeTransfer;
                        }
                        else
                            throw new ArgumentException("Bad message code");
                    }
                    break;
                default:
                    throw new ArgumentException("Bad state");
            }
        }

        public override void OnInput(char c)
        {
            switch (state)
            {
                case TState.EStateTransferActive:
                    {
                        switch (c)
                        {
                            case '0':
                                {
                                    state = TState.GettingDeviceID;
                                }
                                break;
                            case '1':
                                {
                                    if (currentInterval > 1)
                                    {
                                        --currentInterval;
                                        SendMessageToMyself(Message.KMsgSetParameters);
                                    }
                                }
                                break;
                            case '3':
                                {
                                    if (currentInterval < 5)
                                    {
                                        currentInterval++;
                                        SendMessageToMyself(Message.KMsgSetParameters);
                                    }
                                }
                                break;
                            case '\r':
                                {
                                    SuspendTransfer();
                                    SendMessageToMyself(deviceID, Message.KMsgSuspendTransfer);
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                case TState.EStateTransferSuspended:
                    {
                        switch (c)
                        {
                            case '\r':
                                {
                                    SendMessageToMyself(Message.KMsgResumeTransfer);
                                }
                                break;
                            default:
                                break;
                        }
                    }
                // Для других состояний
                default:
                    break;
            }
        }

        public override void OnTerminate()
        {
            // Для всех состояний
            SendMessageToDispatcher(deviceID, Message.KMsgEndSession);
        }

        public override vodi OnSuspend()
        {
            switch (state)
            {
                case TState.EStateTransferActive:
                    {
                        state = TState.EStateTransferSuspended;
                        SendMessageToDispatcher(deviceID, Message.KMsgSuspendTransfer);
                    }
                    break;
                default:
                    break;
            }
        }

        public override void OnDisconnect()
        {
            OnSuspend();
        }

        public override void OnResume()
        {
            switch (state)
            {
                case TState.EStateTransferActive:
                case TState.EStateTransferSuspended:
                    {
                        SendMessageToDispatcher(deviceID, Message.KMsgResumeTransfer);
                    }
                    break;
                default:
                    break;
            }
        }

        public int GetDeviceID();
        public int MainMenu();
        public void SendMessageToMyself(int messageCode);
        public void SendMessageToDispatcher(int deviceID, int messageCode);
        public void ShowInfo(int messageCode);
        public void showError(int errCode);
        public void SetDeviceParameters();
        public void ProcessIncomingMessage(Message msg);
    }
}




    



                    
    

