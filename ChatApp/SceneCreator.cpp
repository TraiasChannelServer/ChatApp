#include "SceneCreator.h"

#include "SceneChoiceHostGuest.h"
#include "SceneHost.h"
#include "SceneGuest.h"

Scene* SceneCreator::Create(Name name)
{
	switch (name)
	{
	case Name::CHOICE_HOST_GUEST:
		return new SceneChoiceHostGuest;
	case Name::HOST:
		return new SceneHost;
	case Name::GUEST:
		return new SceneGuest;
	}
	return nullptr;
}
