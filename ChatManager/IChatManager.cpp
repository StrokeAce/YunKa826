#include "IChatManager.h"
#include "chat_manager/chat_manager.h"

CManagerFactory* CManagerFactory::mpr_factory = nullptr;

IChatManager* CManagerFactory::GetManager()
{
	if (mpr_manager == NULL)
	{
		mpr_manager = new CChatManager();
	}
	return mpr_manager;
}

CManagerFactory::CManagerFactory()
{

}

CManagerFactory* CManagerFactory::GetInstance()
{
	if (mpr_factory == NULL)
	{
		mpr_factory = new CManagerFactory();
	}
	return mpr_factory;
}

