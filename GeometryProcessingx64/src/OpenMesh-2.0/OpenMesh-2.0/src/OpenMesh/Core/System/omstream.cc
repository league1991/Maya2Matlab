/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2010 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------* 
 *  This file is part of OpenMesh.                                           *
 *                                                                           *
 *  OpenMesh is free software: you can redistribute it and/or modify         * 
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenMesh is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenMesh.  If not,                                    *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/ 

/*===========================================================================*\
 *                                                                           *             
 *   $Revision: 325 $                                                         *
 *   $Date: 2010-06-17 12:45:58 +0200 (Do, 17 Jun 2010) $                   *
 *                                                                           *
\*===========================================================================*/

//=============================================================================
//
//  CLASS mostream - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/omstream.hh>


//== IMPLEMENTATION ========================================================== 


OpenMesh::mostream& omlog() 
{
  static bool initialized = false;
  static OpenMesh::mostream mystream;
  if (!initialized)
  {
    mystream.connect(std::clog);
#ifdef NDEBUG
    mystream.disable();
#endif
  }
  return mystream;
}


OpenMesh::mostream& omout() 
{
  static bool initialized = false;
  static OpenMesh::mostream mystream;
  if (!initialized) mystream.connect(std::cout);
  return mystream;
}


OpenMesh::mostream& omerr() 
{
  static bool initialized = false;
  static OpenMesh::mostream mystream;
  if (!initialized) mystream.connect(std::cerr);
  return mystream;
}


//=============================================================================
