module RepoIce
{
	sequence<int> QIntList;
	sequence<string> IdTypeList;

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
		idempotent string getId();	// id 
		
		idempotent string getName() ;	// название 
		idempotent void setName(string name);
		
		idempotent string getDescription() ;	// описание
		idempotent void setDescription(string description);
    
		idempotent void setProperty(string name, string val ); //установить свойство
		idempotent string getProperty(string name ) ; //в случае отсутсвия возвращает пустую строку ""
		idempotent int getPropertiesCount(); // возвращает кол-во свойств
    
		idempotent MetaTypeIce getMetaTypeIce() ; // метатип
		idempotent void setMetaTypeIce(MetaTypeIce mType);
    
		idempotent IdTypeList getObjects() ; // вернуть все объекты этого типа
	};

	interface RealObjectIce
	{
		idempotent string getId();	// id
    
		idempotent string getName() ;	// название 
		idempotent void setName(string name);
    
		idempotent string getDescription() ;	// описание
		idempotent void setDescription(string descritption);
    
		idempotent void setProperty(string name, string val ); //установить свойство
		idempotent string getProperty(string name ) ; //в случае отсутствия возвращает пустую строку ""
		idempotent int getPropertiesCount(); // возвращает кол-во свойств
    
		idempotent string getTypeId() ; // тип
		idempotent void setTypeId(string id);

		idempotent bool getVisibility() ; // видим ли на диаграмме
		idempotent void setVisibility(bool is);
    
		idempotent IdTypeList getContainerId() ; // возвращает id всех контейнеров, которым принадлежит объект
		idempotent void addToContainer(string id); // добавляет объект в указанный контейнер, вместо setContainerId, который deprecated
		idempotent void deleteFromContainer(string id); // удаляет объект из контейнера

		idempotent string getConfiguration() ; // конфигурация
		idempotent void setConfiguration(string configuration);
    
		idempotent IdTypeList getChildElements() ; // коллекция id внутренних элементов
		void addChildElement(string elementId);
		void deleteChildElement(string elementId);
    
		idempotent IdTypeList getAllLinks() ; // получить линки
		idempotent IdTypeList getIncomingLinks() ;
		idempotent IdTypeList getOutcomingLinks() ;
		
		void addIncomingLink(string linkId);
		void addOutcomingLink(string linkId);

		void removeIncomingLink(string linkId);
		void removeOutcomingLink(string linkId);
	};

	interface RealLinkIce
	{
		idempotent string getId();
		idempotent void setName(string name);
		idempotent string getName();

		idempotent void setProperty(string name, string val ); //установить свойство
		idempotent string getProperty(string name ); //в случае отсутсвия возвращает пустую строку ""
		idempotent int getPropertiesCount(); // возвращает кол-во свойств
		
		idempotent string getFromId();	// элемент-источник
		idempotent void setFromId(string id);

		idempotent string getToId() ;	// элемент-приёмник
		idempotent void setToId(string id);
	};

	interface RepoClientIce
	{
		idempotent IdTypeList getAllTypes(); // вернуть все типы
		idempotent IdTypeList getTypesByMetaTypeIce(MetaTypeIce mType); // вернуть типы по метатипу
		idempotent RealTypeIce* getTypeById(string id); // вернуть тип по id
		idempotent RealTypeIce* getTypeByName(string name); // вернуть тип по имени
		idempotent string getTypeIdByName(string name); // вернуть id типа по имени
		
		idempotent IdTypeList getObjectsListByType(string id); // вернуть все объекты конкретного типа
		idempotent IdTypeList getLinks(); //вернуть все линки


		idempotent RealObjectIce* getObjectById(string id); // вернуть по id
		string createObject(string type, string name); //создать. Возвращает Id созданного.
		string createObjectWithParent(string type, string name, string parent); // создаёт объект и при этом помещает его в указанный контейнер
		void deleteObject(string id, string parent); //удаляет объект совсем из репозитария. Если он содержался в каких-то контейнерах, то сначала удаляет его из них, потом из репозитария

		idempotent RealLinkIce* getLinkById(string id); // вернуть по id
		string createLink(string name); //создать. Возвращает Id созданного.
		string createLinkWithType(string name, string type); //создать. Возвращает Id созданного.
		string createLinkWithParent(string type, string name, string parent); // создаёт линку и при этом помещает её в указанный контейнер
		void deleteLink(string id, string parent); // удаляет объект совсем из репозитария. Если он содержался в каких-то контейнерах, то сначала удаляет его из них, потом из репозитария
	};
};

