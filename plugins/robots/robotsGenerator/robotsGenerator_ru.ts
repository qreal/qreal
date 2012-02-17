<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ru_RU">
<context>
    <name>QObject</name>
    <message>
        <location filename="nxtOSEK/funcOrientedGenerator/funcOrientedGenerator.cpp" line="54"/>
        <location filename="nxtOSEK/funcOrientedGenerator/funcOrientedGenerator.cpp" line="69"/>
        <location filename="nxtOSEK/funcOrientedGenerator/funcOrientedGenerator.cpp" line="77"/>
        <location filename="nxtOSEK/funcOrientedGenerator/funcOrientedGenerator.cpp" line="94"/>
        <location filename="nxtOSEK/funcOrientedGenerator/funcOrientedGenerator.cpp" line="102"/>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="94"/>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="109"/>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="125"/>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="133"/>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="150"/>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="158"/>
        <source>cannot open &quot;%1&quot;</source>
        <oldsource>cannot open &quot;</oldsource>
        <translation>невозможно открыть &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="177"/>
        <source>There is nothing to generate, diagram doesn&apos;t have Initial Node</source>
        <translation>Генерация невозможна, на диаграмме нет начальных блоков</translation>
    </message>
    <message>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="549"/>
        <source>Error! There are more than 1 outgoing connected elements with simple robot element!</source>
        <translation>Ошибка! Больше одной исходящей связи из простого блока!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="586"/>
        <source>Element %1 has no correct next element because its link has no end object. May be you need to connect it to diagram object.</source>
        <translation>У элемента %1 нет корректного следующего, так как его исходящая связь ни на что не показывает. Надо соединить её с некоторым элементом диаграммы.</translation>
    </message>
    <message>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="739"/>
        <source>Link from %1 has no object on its end. May be you need to connect it to diagram object.</source>
        <translation>Исходящая связь %1 ни на что не указывает. Надо соединить её с некоторым элементом диаграммы.</translation>
    </message>
    <message>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="607"/>
        <source>Error! There is no outgoing connected elements with no final node!</source>
        <translation>Ошибка! У не конечного элемента не задан следующий!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/funcOrientedGenerator/elementGenerators/loop_gen.cpp" line="12"/>
        <source>Less than 2 outgoing elements for loop element!</source>
        <translation>Менее 2 исходящих связей для блока цикла!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/funcOrientedGenerator/elementGenerators/loop_gen.cpp" line="31"/>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="634"/>
        <source>Loop block %1 has no correct loop branch! May be you need to connect it to some diagram element.</source>
        <translation>У блока цикла %1 некорректно задана ветвь цикла! Возможно надо поправить связи со следующими элементами.</translation>
    </message>
    <message>
        <location filename="nxtOSEK/funcOrientedGenerator/elementGenerators/loop_gen.cpp" line="52"/>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="656"/>
        <source>Loop block %1 has no correct next block branch! May be you need to connect it to some diagram element.</source>
        <translation>У блока цикла %1 некорректно задана ветвь после него! Возможно надо поправить связи со следующими элементами.</translation>
    </message>
    <message>
        <location filename="nxtOSEK/funcOrientedGenerator/elementGenerators/ifBlock_gen.cpp" line="10"/>
        <source>Less than 2 outgoing elements for if element!</source>
        <translation>Менее 2 исходящих связей для условного блока!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/funcOrientedGenerator/elementGenerators/ifBlock_gen.cpp" line="33"/>
        <location filename="nxtOSEK/funcOrientedGenerator/elementGenerators/ifBlock_gen.cpp" line="45"/>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="684"/>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="789"/>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="803"/>
        <source>If block %1 has no 2 correct branches! May be you need to connect one of them to some diagram element.</source>
        <translation>У условного блока %1 нет 2 корректных следующих элементов. Возможно надо поправить связи со следующими элементами.</translation>
    </message>
    <message>
        <location filename="nxtOSEK/sequentialGenerator/sequentialGenerator.cpp" line="820"/>
        <source>This diagram isn&apos;t structed diagram, because there are IF block with 2 back arrows!</source>
        <translation>Данная диаграмма не структурная, так как в ней есть условный блок (IF) с 2 ветками, ведущими к предыдущим элементам!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/funcOrientedGenerator/elementGenerators/simpleElementGenerator.cpp" line="14"/>
        <source>Too many outgoing connections!</source>
        <translation>Слишком много исходящих связей!</translation>
    </message>
</context>
<context>
    <name>robots::generator::NxtFlashTool</name>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="41"/>
        <source>Firmware flash started. Please don&apos;t disconnect robot during the process</source>
        <translation>Начат процесс загрузки прошивки в робота. Пожалуйста, не отсоединяйте робота, пока процесс не будет завершен</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="47"/>
        <source>Some error occured. Make sure you are running QReal with superuser privileges</source>
        <translation>Ошибка. Убедитесь, что QReal:Robots запущен с правами администратора</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="55"/>
        <source>Flashing process completed.</source>
        <translation>Процесс загрузки программы завершен.</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="57"/>
        <source>flash.sh not found. Make sure it is present in QReal installation directory</source>
        <translation>Не найден скрипт flash.sh. Убедитесь, что пакет nxt-tools установлен корректно</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="59"/>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="101"/>
        <source>QReal requires superuser privileges to flash NXT robot</source>
        <translation>Для загрузки прошивки в робота требуется запустить QReal:Robots с правами администратора</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="72"/>
        <source>NXT not found. Check USB connection and make sure the robot is ON</source>
        <translation>Соединение с роботом не обнаружено. Проверьте USB-соединение и убедитесь, что робот включен</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="74"/>
        <source>NXT is not in reset mode. Please reset your NXT manually and try again</source>
        <translation>Робот находится в неправильном режиме. Переведите робота в режим перепрошивки</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="76"/>
        <source>Firmware flash complete!</source>
        <translation>Процесс загрузки прошивки в робота завершен!</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="91"/>
        <source>Uploading program started. Please don&apos;t disconnect robot during the process</source>
        <translation>Начат процесс загрузки программы в робота. Пожалуйста, не отсоединяйте робота, пока процесс не будет завершен</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="99"/>
        <source>Uploading failed. Make sure that X-server allows root to run GUI applications</source>
        <translation>Не удалось загрузить программу в робота. Убедитесь, что QReal:Robots запущен с нужными правами</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="129"/>
        <source>Could not upload program. Make sure the robot is connected and ON</source>
        <translation>Не удалось загрузить программу в робота. Убедитесь, что робот включен и подсоединен к компьютеру</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="131"/>
        <source>Uploading completed successfully</source>
        <translation>Загрузка программы завершена успешно</translation>
    </message>
    <message>
        <location filename="nxtOSEK/nxtFlashTool.cpp" line="136"/>
        <source>QReal requires superuser privileges to upload programs on NXT robot</source>
        <translation>Для загрузки программ в робота требуется запустить QReal:Robots с правами администратора</translation>
    </message>
</context>
<context>
    <name>robots::generator::RobotsGeneratorPlugin</name>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="39"/>
        <source>Generate code</source>
        <translation>Генерировать код</translation>
    </message>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="43"/>
        <source>Toggle generators</source>
        <translation>Переключить генераторы</translation>
    </message>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="47"/>
        <source>Flash robot</source>
        <translation>Прошить</translation>
    </message>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="51"/>
        <source>Upload program</source>
        <translation>Загрузить программу</translation>
    </message>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="88"/>
        <source>flash.sh not found. Make sure it is present in QReal installation directory</source>
        <translation>Не найден скрипт flash.sh. Убедитесь, что пакет nxt-tools установлен корректно</translation>
    </message>
    <message>
        <location filename="robotsGeneratorPlugin.cpp" line="97"/>
        <source>upload.sh not found. Make sure it is present in QReal installation directory</source>
        <translation>Не найден скрипт upload.sh. Убедитесь, что пакет nxt-tools установлен корректно</translation>
    </message>
</context>
</TS>
