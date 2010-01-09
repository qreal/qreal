module RepoIce
{
	sequence<string> IdList;
	const string ROOTID = "qrm:/ROOT_ID/ROOT_ID/ROOT_ID/ROOT_ID";

	interface RepoApi
	{
		idempotent string name(string id);
		void setName(string id, string name);

		IdList children(string id);
		void addChild(string id, string child);
		void removeChild(string id, string child);

		void removeElement(string id);

		idempotent IdList parents(string id);
		void addParent(string id, string parent);
		void removeParent(string id, string parent);

		idempotent IdList outcomingLinks(string id);
		idempotent IdList incomingLinks(string id);
		idempotent IdList links(string id);

		idempotent string typeName(string id);

		idempotent string property(string id, string propertyName);
		idempotent string stringProperty(string id, string propertyName);
		void setProperty(string id, string propertyName, string value);
		void removeProperty(string id, string propertyName);
		idempotent bool hasProperty(string id, string propertyName);

		idempotent string from(string id);
		void setFrom(string id, string from);

		idempotent string to(string id);
		void setTo(string id, string to);

		idempotent double fromPort(string id);
		void setFromPort(string id, double fromPort);

		idempotent double toPort(string id);
		void setToPort(string id, double toPort);

		idempotent string otherEntityFromLink(string linkId, string firstNode);

		void exterminate();
		void save();
	};
};
