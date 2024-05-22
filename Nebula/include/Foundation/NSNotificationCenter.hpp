#pragma once

#include "NSDefines.hpp"
#include "NSString.hpp"
#include "NSObject.hpp"
#include "NSPrivate.hpp"
#include "NSDictionary.hpp"
#include <Foundation/NSNotification.hpp>
#include <Foundation/NSNumber.hpp>

namespace NS {

class NotificationCenter;
class NotificationObserver
{
public:
    virtual void handleNotification(NS::Notification* pNotification) {};

protected:
    NotificationCenter* m_pNotificationCenter = nullptr;
};

class NotificationCenter : public NS::Referencing<NotificationCenter> {
public:
    static NotificationCenter* defaultCenter();

    void addObserver(NS::NotificationObserver* observer, NS::String* name, NS::Object* object);
    void removeObserver(NS::Object* observer, NS::String* name, NS::Object* object);
    void postNotification(NS::String* name, NS::Object* object, NS::Dictionary* userInfo);
    void setSuspended(bool suspended);
};
}

_NS_INLINE NS::NotificationCenter* NS::NotificationCenter::defaultCenter() {  
    return Object::sendMessage<NotificationCenter*>(_NS_PRIVATE_CLS(NSNotificationCenter), _NS_PRIVATE_SEL(_ns_notification_center_defaultCenter), _NS_PRIVATE_SEL(_ns_notification_center_defaultCenter));
}

_NS_INLINE void NS::NotificationCenter::addObserver(NS::NotificationObserver* pNotificationObserver, NS::String* name, NS::Object* object) {
    NS::Value* pWrapper = NS::Value::value(pNotificationObserver);

    typedef void (*DispatchFunction)( NS::Value*, SEL, void* pNotification );
    DispatchFunction handleNotification = []( Value* pSelf, SEL, void* pNotification ){
		auto pDel = reinterpret_cast< NS::NotificationObserver* >( pSelf->pointerValue() );
		pDel->handleNotification( (NS::Notification *)pNotification );
	};


	class_addMethod( (Class)_NS_PRIVATE_CLS( NSValue ), _NS_PRIVATE_SEL( _ns_notification_handler_handleNotification_ ), (IMP)handleNotification, "v@:@" );
    Object::sendMessage<void>(this, _NS_PRIVATE_SEL(addObserverSelector_), pWrapper, _NS_PRIVATE_SEL(_ns_notification_handler_handleNotification_), name, object);
}

_NS_INLINE void NS::NotificationCenter::removeObserver(NS::Object* observer, NS::String* name, NS::Object* object) {
    Object::sendMessage<void>(this, _NS_PRIVATE_SEL(removeObserverSelector_), observer, name, object);
}

_NS_INLINE void NS::NotificationCenter::postNotification(NS::String* name, NS::Object* object, NS::Dictionary* userInfo) {
    Object::sendMessage<void>(this, _NS_PRIVATE_SEL(postNotificationSelector_), name, object, userInfo);
}

_NS_INLINE void NS::NotificationCenter::setSuspended(bool suspended) {
    Object::sendMessage<void>(this, _NS_PRIVATE_SEL(setSuspendedSelector_), suspended ? YES : NO);
}