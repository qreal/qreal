#include "svnClient.h"

class Listener : public svn::ContextListener
{
public:
	virtual bool contextGetLogin(const std::string & /*realm*/,
					std::string & /*username*/,
					std::string & /*password*/,
					bool & /*maySave*/) { return true; }

	virtual void contextNotify(const char * /*path*/,
				  svn_wc_notify_action_t /*action*/,
				  svn_node_kind_t /*kind*/,
				  const char * /*mime_type*/,
				  svn_wc_notify_state_t /*content_state*/,
				  svn_wc_notify_state_t /*prop_state*/,
				  svn_revnum_t /*revision*/) { }

	virtual bool contextCancel() { return false; }

	virtual bool contextGetLogMessage(std::string & /*msg*/) { return true; }

	virtual SslServerTrustAnswer contextSslServerTrustPrompt(const SslServerTrustData & /*data*/,
								apr_uint32_t & /*acceptedFailures*/) { return SslServerTrustAnswer(); }

	virtual bool contextSslClientCertPrompt(std::string & /*certFile*/) { return true; }

	virtual bool contextSslClientCertPwPrompt(std::string & /*password*/,
								 const std::string & /*realm*/,
								 bool & /*maySave*/) { return true; }
};

SvnClient::SvnClient(char *workingDir, char *username, char *pass)
{
	Listener *listener = new Listener;
	ctx = new Context(workingDir);
	ctx->setLogin(username, pass);
	ctx->setListener(listener);
//	client = new Client(ctx);
	client.setContext(ctx);
	Targets tar(workingDir);
	targets = new Targets(workingDir);
}
int SvnClient::commit(QString &message)
{
	svn_revnum_t revnum;
	try
	{
		revnum = client.commit(*targets, "test", true);
	}
	catch (svn::ClientException e) {
		message = e.message();
//		printf("[%s]\n", e.message());
		return -1;
	}
	return revnum;
}
void SvnClient::checkout()
{
}
