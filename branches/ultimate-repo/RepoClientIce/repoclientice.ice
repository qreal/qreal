module RepoIce
{
	sequence<int> QIntList;

	// Метатипы
	enum MetaTypeIce
	{
		rObject, //типы объектов на диаграмме (например invoke, exit, if, foreach, throw, reply)
		rLink, //Рёбра
		rDataType, //типы данных (например базовые int, double, string, char и расширенные -- записи (структуры), с описаннием в виде xsd)
		rRawType //типы хранимых RAW данных (строк) (например wsdl, xsd, параметры проекта)
	};

	// Типы данных 
	interface RealTypeIce
	{
		idempotent int getId();	// id 
		
		idempotent string getName() ;	// название 
		idempotent void setName(string name);
		
		idempotent string getDescription() ;	// описание
		idempotent void setDescription(string description);
    
		idempotent void setProperty(string name, string val ); //установить свойство
		idempotent string getProperty(string name ) ; //в случае отсутсвия возвращает пустую строку ""
		idempotent int getPropertiesCount(); // возвращает кол-во свойств
    
		idempotent MetaTypeIce getMetaTypeIce() ; // метатип
		idempotent void setMetaTypeIce(MetaTypeIce mType);
    
		idempotent QIntList getObjects() ; // вернуть все объекты этого типа
	};

	interface RealObjectIce
	{
		idempotent int getId();	// id 
    
		idempotent string getName() ;	// название 
		idempotent void setName(string name);
    
		idempotent string getDescription() ;	// описание
		idempotent void setDescription(string descritption);
    
		idempotent void setProperty(string name, string val ); //установить свойство
		idempotent string getProperty(string name ) ; //в случае отсутствия возвращает пустую строку ""
		idempotent int getPropertiesCount(); // возвращает кол-во свойств
    
		idempotent int getTypeId() ; // тип
		idempotent void setTypeId(int id);
    
		idempotent bool getVisibility() ; // видим ли на диаграмме
		idempotent void setVisibility(bool is);
    
		idempotent int getContainerId() ; // id контейнера
		idempotent void setContainerId(int id);

		idempotent string getConfiguration() ; // конфигурация
		idempotent void setConfiguration(string configuration);
    
		idempotent QIntList getChildElements() ; // коллекция id внутренних элементов
		void addChildElement(int elementId);
		void deleteChildElement(int elementId);
    
		idempotent QIntList getAllLinks(int direction) ; // получить линки
		idempotent QIntList getIncomingLinks() ;
		idempotent QIntList getOutcomingLinks() ;
		
		void addIncomingLink(int linkId);
		void addOutcomingLink(int linkId);

		void removeIncomingLink(int linkId);
		void removeOutcomingLink(int linkId);
	};

	interface RealLinkIce
	{
		idempotent int getId();
		idempotent void setName(string name);

		idempotent void setProperty(string name, string val ); //установить свойство
		idempotent string getProperty(string name ); //в случае отсутсвия возвращает пустую строку ""
		idempotent int getPropertiesCount(); // возвращает кол-во свойств
		
		idempotent int getFromId();	// элемент-источник
		idempotent void setFromId(int id);

		idempotent int getToId() ;	// элемент-приёмник
		idempotent void setToId(int id);
	};

	interface RepoClientIce
	{
		idempotent QIntList getAllTypes(); // вернуть все типы
		idempotent QIntList getTypesByMetaTypeIce(MetaTypeIce mType); // вернуть типы по метатипу
		idempotent RealTypeIce* getTypeById(int id); // вернуть тип по id
		idempotent RealTypeIce* getTypeByName(string name); // вернуть тип по имени
		idempotent int getTypeIdByName(string name ); // вернуть id типа по имени
		
		idempotent QIntList getObjectsListByType(int id); // вернуть все объекты конкретного типа
		idempotent QIntList getLinks(); //вернуть все линки

		//int createType(string name); //создать тип. Возвращает Id созданного типа.
		//void deleteType(int id); //удалить тип

		idempotent RealObjectIce* getObjectById(int id); // вернуть по id
		int createObject(int type, string name); //создать. Возвращает Id созданного.
		int createObjectWithParent(int type, string name, int parent); //создать. Возвращает Id созданного.
		void deleteObject(int id); //удалить

		idempotent RealLinkIce* getLinkById(int id); // вернуть по id
		int createLink(string name); //создать. Возвращает Id созданного.
		void deleteLink(int id); //удалить
	};
};

