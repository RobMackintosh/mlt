/*
 * mlt_field.c -- A field for planting multiple transitions and filters
 * Copyright (C) 2003-2004 Ushodaya Enterprises Limited
 * Author: Charles Yates <charles.yates@pandora.be>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "mlt_field.h"
#include "mlt_service.h"
#include "mlt_filter.h"
#include "mlt_transition.h"
#include "mlt_multitrack.h"
#include "mlt_tractor.h"

#include <stdlib.h>
#include <string.h>

/** Private structures.
*/

struct mlt_field_s
{
	// This is the producer we're connected to
	mlt_service producer;

	// Multitrack
	mlt_multitrack multitrack;

	// Tractor
	mlt_tractor tractor;
};

/** Constructor.
	
  	We construct a multitrack and a tractor here.
*/

mlt_field mlt_field_init( )
{
	// Initialise the field
	mlt_field this = calloc( sizeof( struct mlt_field_s ), 1 );

	// Initialise it
	if ( this != NULL )
	{
		// Construct a multitrack
		this->multitrack = mlt_multitrack_init( );

		// Construct a tractor
		this->tractor = mlt_tractor_init( );

		// The first plant will be connected to the mulitrack
		this->producer = mlt_multitrack_service( this->multitrack );

		// Connect the tractor to the multitrack
		mlt_tractor_connect( this->tractor, this->producer );
	}

	// Return this
	return this;
}

/** Get the service associated to this field.
*/

mlt_service mlt_field_service( mlt_field this )
{
	return mlt_tractor_service( this->tractor );
}

/** Get the multi track.
*/

mlt_multitrack mlt_field_multitrack( mlt_field this )
{
	return this->multitrack;
}

/** Get the tractor.
*/

mlt_tractor mlt_field_tractor( mlt_field this )
{
	return this->tractor;
}

/** Get the properties associated to this field.
*/

mlt_properties mlt_field_properties( mlt_field this )
{
	return mlt_service_properties( mlt_field_service( this ) );
}

/** Plant a filter.
*/

int mlt_field_plant_filter( mlt_field this, mlt_filter that, int track )
{
	// Connect the filter to the last producer
	int result = mlt_filter_connect( that, this->producer, track );

	// If sucessful, then we'll use this for connecting in the future
	if ( result == 0 )
	{
		// This is now the new producer
		this->producer = mlt_filter_service( that );

		// Reconnect tractor to new producer
		mlt_tractor_connect( this->tractor, this->producer );
	}

	return result;
}

/** Plant a transition.
*/

int mlt_field_plant_transition( mlt_field this, mlt_transition that, int a_track, int b_track )
{
	// Connect the transition to the last producer
	int result = mlt_transition_connect( that, this->producer, a_track, b_track );

	// If sucessful, then we'll use this for connecting in the future
	if ( result == 0 )
	{
		// This is now the new producer
		this->producer = mlt_transition_service( that );

		// Reconnect tractor to new producer
		mlt_tractor_connect( this->tractor, this->producer );
	}

	return 0;
}

/** Close the field.
*/

void mlt_field_close( mlt_field this )
{
	free( this );
}

