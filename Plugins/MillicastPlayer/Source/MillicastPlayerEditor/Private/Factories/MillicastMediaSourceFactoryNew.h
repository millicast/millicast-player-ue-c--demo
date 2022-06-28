// Copyright CoSMoSOftware 2021. All Rights Reserved.

#pragma once

#include "Factories/Factory.h"
#include "MillicastMediaSourceFactoryNew.generated.h"


/**
 * Implements a factory for objects.
 */
UCLASS(hidecategories=Object)
class UMillicastMediaSourceFactoryNew
	: public UFactory
{
	GENERATED_UCLASS_BODY()

public:

	//~ UFactory Interface

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual uint32 GetMenuCategories() const override;
	virtual FText GetDisplayName() const override;
	virtual bool ShouldShowInNewMenu() const override;
};
