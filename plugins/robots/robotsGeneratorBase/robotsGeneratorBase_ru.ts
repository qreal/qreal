<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ru_RU">
<context>
    <name>QObject</name>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="29"/>
        <source>There is nothing to generate, diagram doesn&apos;t have Initial Node</source>
        <translation>Генерация невозможна, на диаграмме нет начальных блоков</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="58"/>
        <source>This element must have exactly ONE outgoing link</source>
        <translation>От этого элемента должна отходить в точности одна связь</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="68"/>
        <source>Final node must not have outgoing links</source>
        <oldsource>Final node must not have outgioing links</oldsource>
        <translation>От блока &quot;Конец&quot; не должно отходить связей</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="76"/>
        <source>If block must have exactly TWO outgoing links</source>
        <translation>От условного блока должно отходить в точности две связи</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="91"/>
        <source>Two outgoing links marked with &apos;true&apos; found</source>
        <translation>Обе связи помечены меткой &quot;истина&quot;</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="100"/>
        <source>Two outgoing links marked with &apos;false&apos; found</source>
        <translation>Обе связи помечены меткой &quot;ложь&quot;</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="139"/>
        <source>Loop block must have exactly TWO outgoing links</source>
        <translation>От блока &quot;Цикл&quot; должно отходить в точности две связи</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="153"/>
        <source>Two outgoing links marked with &quot;iteration&quot; found</source>
        <translation>Обе связи помечены меткой &quot;итерация&quot;</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="161"/>
        <source>There must be a link with property &quot;Guard&quot; set to &quot;iteration&quot;</source>
        <translation>Должна быть связь с меткой &quot;итерация&quot;</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="172"/>
        <source>Outgoing links from loop block must be connected to different blocks</source>
        <translation>Исходящие связи должны быть присоединены к разным блокам</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="190"/>
        <source>Fork block must have at least TWO outgoing links</source>
        <translation>Из блок &quot;Параллельные задачи&quot; должно выходить как минимум две связи</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="203"/>
        <source>Unknown block type</source>
        <translation>Неизвестный блок</translation>
    </message>
    <message>
        <source>There is no outgoing connected element with no final node!</source>
        <translation type="obsolete">Отсутствует исходящий элемент!</translation>
    </message>
    <message>
        <source>There are more than 1 outgoing connected elements with simple robot element!</source>
        <translation type="obsolete">Обнаружено более 1 исходящей связи!</translation>
    </message>
    <message>
        <location filename="masterGeneratorBase.cpp" line="30"/>
        <source>There is no opened diagram</source>
        <translation>Сначала откройте диаграмму </translation>
    </message>
    <message>
        <source>The comment block with incomming liks detected!</source>
        <translation type="obsolete">Блок коментария не может иметь входящих связей!</translation>
    </message>
    <message>
        <source>If block </source>
        <translation type="obsolete">У условного блока </translation>
    </message>
    <message>
        <source> has no 2 correct branches! May be you need to connect one of them to some diagram element.</source>
        <translation type="obsolete"> найдены не присоединенные ветки!</translation>
    </message>
    <message>
        <source>There must be exactly TWO links outgoing from if block</source>
        <translation type="obsolete">От условного блока должно отходить ровно ДВЕ связи</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="216"/>
        <source>Outgoing link is not connected</source>
        <translation>Исходящая связь ни к чему не подключена</translation>
    </message>
    <message>
        <source>Two links marked with &apos;true&apos; found</source>
        <translation type="obsolete">Обнаружено обе связи помечены условием &apos;Истина&apos;</translation>
    </message>
    <message>
        <source>Two links marked with &apos;false&apos; found</source>
        <translation type="obsolete">Обе связи помечены условием &apos;Ложь&apos;</translation>
    </message>
    <message>
        <location filename="primaryControlFlowValidator.cpp" line="109"/>
        <source>There must be a link with property &quot;Guard&quot; set to one of the conditions</source>
        <translation>Должна быть связь со свойством &quot;Условие&quot; установленным в одно из условий</translation>
    </message>
    <message>
        <source>This diagram isn&apos;t structed diagram, because there are IF block with 2 back arrows!</source>
        <translation type="obsolete">Диаграмма не структурная т.к. существует условный блок с двумя обратными стрелками!</translation>
    </message>
    <message>
        <source>Generator JUST does not work in this case.</source>
        <translation type="obsolete">Данная диаграмма слишком сложна для генерации.</translation>
    </message>
    <message>
        <location filename="parts/subprograms.cpp" line="53"/>
        <source>Graphical diagram instance not found</source>
        <translation>Графическая модель диаграммы с подпрограммой не найдена</translation>
    </message>
    <message>
        <location filename="parts/subprograms.cpp" line="135"/>
        <source>Please enter valid c-style name for subprogram &quot;</source>
        <translation>Введите корректный идентификатор подпрограммы &quot;</translation>
    </message>
    <message>
        <location filename="parts/subprograms.cpp" line="141"/>
        <source>Duplicate identifier: </source>
        <translation>Такой идентификатор уже используется: </translation>
    </message>
</context>
<context>
    <name>qReal::robots::generators::ReadableControlFlowGenerator</name>
    <message>
        <location filename="readableControlFlowGenerator.cpp" line="163"/>
        <source>This diagram cannot be generated into the structured code</source>
        <translation>Данная диаграмма не может быть сгенерирована в структурированный код</translation>
    </message>
</context>
<context>
    <name>qReal::robots::generators::RobotsGeneratorPluginBase</name>
    <message>
        <location filename="robotsGeneratorPluginBase.cpp" line="36"/>
        <source>Generated code</source>
        <translation>Сгенерированный код</translation>
    </message>
</context>
<context>
    <name>robots::generator::NxtFlashTool</name>
    <message>
        <source>Firmware flash started. Please don&apos;t disconnect robot during the process</source>
        <translation type="obsolete">Начат процесс загрузки прошивки в робота. Пожалуйста, не отсоединяйте робота, пока процесс не будет завершен</translation>
    </message>
    <message>
        <source>Some error occured. Make sure you are running QReal with superuser privileges</source>
        <translation type="obsolete">Ошибка. Убедитесь, что QReal:Robots запущен с правами администратора</translation>
    </message>
    <message>
        <source>Flashing process completed.</source>
        <translation type="obsolete">Процесс загрузки программы завершен.</translation>
    </message>
    <message>
        <source>flash.sh not found. Make sure it is present in QReal installation directory</source>
        <translation type="obsolete">Не найден скрипт flash.sh. Убедитесь, что пакет nxt-tools установлен корректно</translation>
    </message>
    <message>
        <source>QReal requires superuser privileges to flash NXT robot</source>
        <translation type="obsolete">Для загрузки прошивки в робота требуется запустить QReal:Robots с правами администратора</translation>
    </message>
    <message>
        <source>NXT not found. Check USB connection and make sure the robot is ON</source>
        <translation type="obsolete">Соединение с роботом не обнаружено. Проверьте USB-соединение и убедитесь, что робот включен</translation>
    </message>
    <message>
        <source>NXT is not in reset mode. Please reset your NXT manually and try again</source>
        <translation type="obsolete">Робот находится в неправильном режиме. Переведите робота в режим перепрошивки</translation>
    </message>
    <message>
        <source>Firmware flash complete!</source>
        <translation type="obsolete">Процесс загрузки прошивки в робота завершен!</translation>
    </message>
    <message>
        <source>Uploading program started. Please don&apos;t disconnect robot during the process</source>
        <translation type="obsolete">Начат процесс загрузки программы в робота. Пожалуйста, не отсоединяйте робота, пока процесс не будет завершен</translation>
    </message>
    <message>
        <source>Uploading failed. Make sure that X-server allows root to run GUI applications</source>
        <translation type="obsolete">Не удалось загрузить программу в робота. Убедитесь, что QReal:Robots запущен с нужными правами</translation>
    </message>
    <message>
        <source>Could not upload program. Make sure the robot is connected and ON</source>
        <translation type="obsolete">Не удалось загрузить программу в робота. Убедитесь, что робот включен и подсоединен к компьютеру</translation>
    </message>
    <message>
        <source>Uploading completed successfully</source>
        <translation type="obsolete">Загрузка программы завершена успешно</translation>
    </message>
    <message>
        <source>Compilation error occured. Please check your function blocks syntax. If you sure in their validness contact developers</source>
        <translation type="obsolete">Произошла ошибка компиляции. Проверьте синтаксис выражений внутри блоков &quot;Функция&quot;. Если Вы уверены в их корректности, обратитесь к разработчикам</translation>
    </message>
    <message>
        <source>QReal requires superuser privileges to upload programs on NXT robot</source>
        <translation type="obsolete">Для загрузки программ в робота требуется запустить QReal:Robots с правами администратора</translation>
    </message>
</context>
<context>
    <name>robots::generator::RobotsGeneratorPlugin</name>
    <message>
        <source>Generate code</source>
        <translation type="obsolete">Генерировать код</translation>
    </message>
    <message>
        <source>Flash robot</source>
        <translation type="obsolete">Прошить</translation>
    </message>
    <message>
        <source>Upload program</source>
        <translation type="obsolete">Загрузить программу</translation>
    </message>
    <message>
        <source>Upload program to robot</source>
        <translation type="obsolete">Загрузить программу на робота</translation>
    </message>
    <message>
        <source>flash.sh not found. Make sure it is present in QReal installation directory</source>
        <translation type="obsolete">Не найден скрипт flash.sh. Убедитесь, что пакет nxt-tools установлен корректно</translation>
    </message>
    <message>
        <source>upload.sh not found. Make sure it is present in QReal installation directory</source>
        <translation type="obsolete">Не найден скрипт upload.sh. Убедитесь, что пакет nxt-tools установлен корректно</translation>
    </message>
</context>
</TS>
