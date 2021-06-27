//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2019 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#ifndef SAMPLE_CHARACTER_CLOTH_PLATFORM_H
#define SAMPLE_CHARACTER_CLOTH_PLATFORM_H

#include "PhysXSample.h"
#include "characterkinematic/PxController.h" 
#include "SampleAllocator.h"
#include "SampleAllocatorSDKClasses.h"

///////////////////////////////////////////////////////////////////////////////
class SampleCharacterClothPlatform : public SampleAllocateable
{
public:

	///////////////////////////////////////////////////////////////////////////////
	enum {
		ePLATFORM_TYPE_STATIC,
		ePLATFORM_TYPE_MOVING,
		ePLATFORM_TYPE_TRIGGERED,
	};

	///////////////////////////////////////////////////////////////////////////////
	SampleCharacterClothPlatform();
	~SampleCharacterClothPlatform() {}

	///////////////////////////////////////////////////////////////////////////////
	PX_FORCE_INLINE	PxRigidDynamic*	getPhysicsActor() { return mActor;}
	PX_FORCE_INLINE	PxReal getTravelTime() const { return mTravelTime; }
	PX_FORCE_INLINE PxU32 getType() const { return mType; }

	PX_FORCE_INLINE void   setActive(bool b) { mActive = b; }
	PX_FORCE_INLINE	void   setTravelTime(PxReal t) { mTravelTime = t; }
	PX_FORCE_INLINE void   setType(PxU32 t) { mType = t; }

	///////////////////////////////////////////////////////////////////////////////
	void init(PxRigidDynamic* actor, PxReal travelTime, const PxVec3& offsetTarget = PxVec3(0,0,0) );
	void release();
	void reset();
	void update(PxReal dtime);

protected:
	PxRigidDynamic*		mActor;
	PxReal				mCurrentTime;
	PxReal				mTravelTime;
	PxVec3              mStart;
	PxVec3              mTarget;
	bool                mFlip;
	PxU32               mType;
	bool                mActive;
};

#endif
