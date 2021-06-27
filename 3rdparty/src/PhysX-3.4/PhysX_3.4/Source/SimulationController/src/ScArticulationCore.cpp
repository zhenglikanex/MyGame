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


#include "ScArticulationCore.h"

#include "PsFoundation.h"
#include "ScPhysics.h"
#include "ScBodyCore.h"
#include "ScArticulationSim.h"
#include "DyArticulation.h"

using namespace physx;

Sc::ArticulationCore::ArticulationCore() :
	mSim(NULL)
{
	const PxTolerancesScale& scale = Physics::getInstance().getTolerancesScale();

	mCore.internalDriveIterations	= 4;
	mCore.externalDriveIterations	= 4;
	mCore.maxProjectionIterations	= 4;
	mCore.separationTolerance		= 0.1f * scale.length;
	mCore.solverIterationCounts		= 1<<8 | 4;
	mCore.sleepThreshold			= 5e-5f * scale.speed * scale.speed;
	mCore.wakeCounter				= Physics::sWakeCounterOnCreation;
	mCore.freezeThreshold			= 5e-6f * scale.speed * scale.speed;
}


Sc::ArticulationCore::~ArticulationCore()
{
}


//--------------------------------------------------------------
//
// ArticulationCore interface implementation
//
//--------------------------------------------------------------

PxU32 Sc::ArticulationCore::getInternalDriveIterations() const
{
	return mCore.internalDriveIterations;
}

void Sc::ArticulationCore::setInternalDriveIterations(const PxU32 v)
{
	mCore.internalDriveIterations = v;
}

PxU32 Sc::ArticulationCore::getExternalDriveIterations() const
{
	return mCore.externalDriveIterations;
}

void Sc::ArticulationCore::setExternalDriveIterations(const PxU32 v)
{
	mCore.externalDriveIterations = v;
}

PxU32 Sc::ArticulationCore::getMaxProjectionIterations() const
{
	return mCore.maxProjectionIterations;
}

void Sc::ArticulationCore::setMaxProjectionIterations(const PxU32 v)
{
	mCore.maxProjectionIterations = v;
}

PxReal Sc::ArticulationCore::getSeparationTolerance() const
{
	return mCore.separationTolerance;
}

void Sc::ArticulationCore::setSeparationTolerance(const PxReal v)
{
	mCore.separationTolerance = v;
}

PxReal Sc::ArticulationCore::getWakeCounter() const
{
	return mCore.wakeCounter;
}

void Sc::ArticulationCore::setWakeCounterInternal(const PxReal v)
{
	mCore.wakeCounter = v;
}

void Sc::ArticulationCore::setWakeCounter(const PxReal v)
{
	mCore.wakeCounter = v;

#ifdef _DEBUG
	if(mSim)
		mSim->debugCheckWakeCounterOfLinks(v);
#endif
}

bool Sc::ArticulationCore::isSleeping() const
{
	return mSim ? mSim->isSleeping() : (mCore.wakeCounter == 0.0f);
}

void Sc::ArticulationCore::wakeUp(PxReal wakeCounter)
{
	mCore.wakeCounter = wakeCounter;

#ifdef _DEBUG
	if(mSim)
		mSim->debugCheckSleepStateOfLinks(false);
#endif
}

void Sc::ArticulationCore::putToSleep()
{
	mCore.wakeCounter = 0.0f;

#ifdef _DEBUG
	if(mSim)
		mSim->debugCheckSleepStateOfLinks(true);
#endif
}

PxReal Sc::ArticulationCore::getSleepThreshold() const
{
	return mCore.sleepThreshold;
}

void Sc::ArticulationCore::setSleepThreshold(const PxReal v)
{
	mCore.sleepThreshold = v;
}

PxReal Sc::ArticulationCore::getFreezeThreshold() const
{
	return mCore.freezeThreshold;
}

void Sc::ArticulationCore::setFreezeThreshold(const PxReal v)
{
	mCore.freezeThreshold = v;
}

PxU16 Sc::ArticulationCore::getSolverIterationCounts() const
{
	return mCore.solverIterationCounts;
}

void Sc::ArticulationCore::setSolverIterationCounts(const PxU16 v)
{
	mCore.solverIterationCounts = v;
}


PxArticulation* Sc::ArticulationCore::getPxArticulation()
{
	return gOffsetTable.convertScArticulation2Px(this);
}


const PxArticulation* Sc::ArticulationCore::getPxArticulation() const
{
	return gOffsetTable.convertScArticulation2Px(this);
}


Sc::ArticulationDriveCache* Sc::ArticulationCore::createDriveCache(PxReal compliance,
												  PxU32 driveIterations) const
{
	return mSim? mSim->createDriveCache(compliance, driveIterations) : NULL;
}


void Sc::ArticulationCore::updateDriveCache(ArticulationDriveCache& cache,
											PxReal compliance,
											PxU32 driveIterations) const
{
	mSim->updateDriveCache(cache, compliance, driveIterations);
}


void Sc::ArticulationCore::releaseDriveCache(Sc::ArticulationDriveCache& driveCache) const
{
	if(mSim)
		mSim->releaseDriveCache(driveCache);
}


PxU32 Sc::ArticulationCore::getCacheLinkCount(const ArticulationDriveCache& cache) const
{
	return Dy::PxvArticulationDriveCache::getLinkCount(cache);
}

void Sc::ArticulationCore::applyImpulse(Sc::BodyCore& link,
										const Sc::ArticulationDriveCache& driveCache,
										const PxVec3& force,
										const PxVec3& torque)
{
	if(mSim)
		mSim->applyImpulse(link, driveCache, force, torque);
}

void Sc::ArticulationCore::computeImpulseResponse(Sc::BodyCore& link,
												  PxVec3& linearResponse, 
												  PxVec3& angularResponse,
												  const Sc::ArticulationDriveCache& driveCache,
												  const PxVec3& force,
												  const PxVec3& torque) const
{
	if(mSim)
		mSim->computeImpulseResponse(link, linearResponse, angularResponse, driveCache, force, torque);
}

IG::NodeIndex Sc::ArticulationCore::getIslandNodeIndex() const
{
	if (mSim)
		return mSim->getIslandNodeIndex();
	return IG::NodeIndex(IG_INVALID_NODE);
}
