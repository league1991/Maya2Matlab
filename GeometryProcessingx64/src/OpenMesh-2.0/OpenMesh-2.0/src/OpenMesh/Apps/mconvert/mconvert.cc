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

#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
//
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Attributes.hh>
#include <OpenMesh/Tools/Utils/Timer.hh>
#include <OpenMesh/Tools/Utils/getopt.h>


struct MyTraits : public OpenMesh::DefaultTraits
{
  VertexAttributes  ( OpenMesh::Attributes::Normal       |
		      OpenMesh::Attributes::Color        |
                      OpenMesh::Attributes::TexCoord2D   );
  HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );
  FaceAttributes    ( OpenMesh::Attributes::Normal       |
		      OpenMesh::Attributes::Color        );
};

  
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;

void usage_and_exit(int xcode)
{
   using std::cout;
   using std::endl;
   
   cout << "\nUsage: mconvert [option] <input> [<output>]\n\n";
   cout << "   Convert from one 3D geometry format to another.\n"
        << "   Or simply display some information about the object\n"
        << "   stored in <input>.\n"
        << endl;
   cout << "Options:\n"
        << endl;
   cout << "  -b\tUse binary mode if supported by target format.\n" << endl;
   cout << "    -l\tStore least significant bit first (LSB, little endian).\n" << endl;
   cout << "    -m\tStore most significant bit first (MSB, big endian).\n" << endl;
   cout << "    -s\tSwap byte order.\n" << endl;
   cout << "  -B\tUse binary mode if supported by source format.\n" << endl;
   cout << "    -S\tSwap byte order of input data.\n" << endl;
   cout << "  -c\tCopy vertex color if provided by input.\n" << endl;
   cout << "  -d\tCopy face color if provided by input.\n" << endl;
   cout << "  -C\tTranslate object in its center-of-gravity.\n" << endl;
   cout << "  -n\tCopy vertex normals if provided by input. Else compute normals.\n" << endl;
   cout << "  -N\tReverse normal directions.\n" << endl;
   cout << "  -t\tCopy vertex texture coordinates if provided by input file.\n" << endl;
   cout << "  -T \"x y z\"\tTranslate object by vector (x, y, z)'\"\n"
        << std::endl;
   cout << endl;
   
   exit(xcode);
}

// ----------------------------------------------------------------------------

template <typename T> struct Option : std::pair< T, bool >
{
  typedef std::pair< T, bool >              Base;
  
  Option()
  { Base::second = false; }

  bool is_valid() const { return Base::second; }
  bool is_empty() const { return !Base::second; }

  operator T& () { return Base::first; }
  operator const T& () const { return Base::first; }

  Option& operator = ( const T& _rhs )
  { Base::first = _rhs; Base::second=true; return *this; }

  bool operator == ( const T& _rhs ) const
  { return Base::first == _rhs; }

  bool operator != ( const T& _rhs ) const
  { return Base::first != _rhs; }
};

template <typename T>
std::ostream& operator << (std::ostream& _os, const Option<T>& _opt )
{
  if (_opt.second) _os << _opt.first; else _os << "<null>";
  return _os;
}

template <typename T>
std::istream& operator >> (std::istream& _is, Option<T>& _opt )
{
  _is >> _opt.first; _opt.second = true;
  return _is;
}

// ----------------------------------------------------------------------------

int main(int argc, char *argv[] )
{
  // ------------------------------------------------------------ command line

  int c;
  std::string ifname, ofname;
  bool rev_normals = false;
  bool obj_center  = false;
  OpenMesh::IO::Options opt, ropt;

  Option< MyMesh::Point > tvec;

  while ( (c=getopt(argc, argv, "bBcdCi:hlmnNo:sStT:"))!=-1 )
  {
    switch(c)
    {
      case 'b': opt  += OpenMesh::IO::Options::Binary; break;
      case 'B': ropt += OpenMesh::IO::Options::Binary; break;
      case 'l': opt  += OpenMesh::IO::Options::LSB; break;
      case 'm': opt  += OpenMesh::IO::Options::MSB; break;
      case 's': opt  += OpenMesh::IO::Options::Swap; break;
      case 'S': ropt += OpenMesh::IO::Options::Swap; break;
      case 'n': opt  += OpenMesh::IO::Options::VertexNormal; break;
      case 'N': rev_normals = true; break;
      case 'C': obj_center  = true; break;
      case 'c': opt  += OpenMesh::IO::Options::VertexColor; break;
      case 'd': opt  += OpenMesh::IO::Options::FaceColor; break;
      case 't': opt  += OpenMesh::IO::Options::VertexTexCoord; break;
      case 'T': 
      {
        std::cout << optarg << std::endl;
        std::stringstream str; str << optarg;
        str >> tvec;
        std::cout << tvec << std::endl;
        break;
      }
      case 'i': ifname = optarg; break;
      case 'o': ofname = optarg; break;
      case 'h':
        usage_and_exit(0);
      case '?':
      default:
        usage_and_exit(1);
    }
  }

  if (ifname.empty())
  { 
    if (optind < argc)
      ifname = argv[optind++];
    else
      usage_and_exit(1);
  }

  MyMesh mesh;
  OpenMesh::Utils::Timer  timer;

  // ------------------------------------------------------------ read

  std::cout << "reading.." << std::endl;
  {
    bool rc;
    timer.start();
    rc = OpenMesh::IO::read_mesh( mesh, ifname, ropt );
    timer.stop();
    if (rc)
      std::cout << "  read in " << timer.as_string() << std::endl;
    else
    {
      std::cout << "  read failed\n" << std::endl;
      return 1;
    }       
    timer.reset();
  }


  // ---------------------------------------- some information about input
  std::cout << (ropt.check(OpenMesh::IO::Options::Binary) 
                ? "  source is binary\n"
                : "  source is ascii\n");   

  std::cout << "  #V " << mesh.n_vertices() << std::endl;
  std::cout << "  #E " << mesh.n_edges() << std::endl;
  std::cout << "  #F " << mesh.n_faces() << std::endl;

  if (ropt.vertex_has_texcoord())
    std::cout << "  has texture coordinates" << std::endl;
    
  if (ropt.vertex_has_normal())
    std::cout << "  has vertex normals" << std::endl;

  if (ropt.vertex_has_color())
    std::cout << "  has vertex colors" << std::endl;
    
  if (ropt.face_has_normal())
    std::cout << "  has face normals" << std::endl;

  if (ropt.face_has_color())
    std::cout << "  has face colors" << std::endl;

  // 
  if (ofname.empty())
  {
    if ( optind < argc )
      ofname = argv[optind++];
    else
      return 0;
  }

  // ------------------------------------------------------------ features

  // ---------------------------------------- compute normal feature
  if ( opt.vertex_has_normal() && !ropt.vertex_has_normal())
  {
    std::cout << "compute normals" << std::endl;

    timer.start();
    mesh.update_face_normals();
    timer.stop();
    std::cout << "  " << mesh.n_faces()
              << " face normals in " << timer.as_string() << std::endl;
    timer.reset();
       
    timer.start();
    mesh.update_vertex_normals();
    timer.stop();
    std::cout << "  " << mesh.n_vertices()
              << " vertex normals in " << timer.as_string() << std::endl;
    timer.reset();       
  }


  // ---------------------------------------- reverse normal feature
  if ( rev_normals && ropt.vertex_has_normal() )
  {
    std::cout << "reverse normal directions" << std::endl;
    timer.start();
    MyMesh::VertexIter vit = mesh.vertices_begin();
    for (; vit != mesh.vertices_end(); ++vit)
      mesh.set_normal( vit.handle(), -mesh.normal( vit.handle() ) );
    timer.stop();
    std::cout << "  " << mesh.n_vertices()
              << " vertex normals in " << timer.as_string() << std::endl;
    timer.reset();       
       
  }


  // ---------------------------------------- centering feature
  if ( obj_center )
  {
    OpenMesh::Vec3f cog(0,0,0);
    size_t nv;
    std::cout << "center object" << std::endl;
    timer.start();    
    MyMesh::VertexIter vit = mesh.vertices_begin();
    for (; vit != mesh.vertices_end(); ++vit)
      cog += mesh.point( vit );
    timer.stop();
    nv   = mesh.n_vertices();
    cog *= 1.0f/mesh.n_vertices();
    std::cout << "  cog = [" << cog << "]'" << std::endl;
    if (cog.sqrnorm() > 0.8) // actually one should consider the size of object
    {
      vit = mesh.vertices_begin();
      timer.cont();
      for (; vit != mesh.vertices_end(); ++vit)
        mesh.set_point( vit , mesh.point( vit )-cog );
      timer.stop();
      nv += mesh.n_vertices();
    }
    else
      std::cout << "    already centered!" << std::endl;
    std::cout << "  visited " << nv
              << " vertices in " << timer.as_string() << std::endl;
    timer.reset();       
  }


  // ---------------------------------------- translate feature
  if ( tvec.is_valid() )
  {
    std::cout << "Translate object by " << tvec << std::endl;

    timer.start();
    MyMesh::VertexIter vit = mesh.vertices_begin();
    for (; vit != mesh.vertices_end(); ++vit)
      mesh.set_point( vit , mesh.point( vit ) + tvec.first );
    timer.stop();
    std::cout << "  moved " << mesh.n_vertices()
              << " vertices in " << timer.as_string() << std::endl;
  }

  // ---------------------------------------- color vertices feature
  if (  opt.check( OpenMesh::IO::Options::VertexColor ) &&
        !ropt.check( OpenMesh::IO::Options::VertexColor ) )
  {
    std::cout << "Color vertices" << std::endl;

    double d  = 256.0/double(mesh.n_vertices());
    double d2 = d/2.0;
    double r  = 0.0, g = 0.0, b = 255.0;
    timer.start();
    MyMesh::VertexIter vit = mesh.vertices_begin();
    for (; vit != mesh.vertices_end(); ++vit)
    {
      mesh.set_color( vit , MyMesh::Color( std::min((int)(r+0.5),255), 
                                           std::min((int)(g+0.5),255), 
                                           std::max((int)(b+0.5),0) ) );
      r += d;
      g += d2;
      b -= d;
    }
    timer.stop();
    std::cout << "  colored " << mesh.n_vertices()
              << " vertices in " << timer.as_string() << std::endl;
  }

  // ---------------------------------------- color faces feature
  if (  opt.check( OpenMesh::IO::Options::FaceColor ) &&
        !ropt.check( OpenMesh::IO::Options::FaceColor ) )
  {
    std::cout << "Color faces" << std::endl;

    double d  = 256.0/double(mesh.n_faces());
    double d2 = d/2.0;
    double r  = 0.0, g = 50.0, b = 255.0;
    timer.start();
    MyMesh::FaceIter it = mesh.faces_begin();
    for (; it != mesh.faces_end(); ++it)
    {
      mesh.set_color( it , MyMesh::Color( std::min((int)(r+0.5),255), 
                                          std::min((int)(g+0.5),255), 
                                          std::max((int)(b+0.5),0) ) );
      r += d2;
//       g += d2;
      b -= d;
    }
    timer.stop();
    std::cout << "  colored " << mesh.n_faces()
              << " faces in " << timer.as_string() << std::endl;
  }

  // ------------------------------------------------------------ write
  
  std::cout << "writing.." << std::endl;    
  {
    bool rc;
    timer.start();
    rc = OpenMesh::IO::write_mesh( mesh, ofname, opt );          
    timer.stop();
       
    if (!rc)
    {
      std::cerr << "  error writing mesh!" << std::endl;
      return 1;
    }
       
    // -------------------------------------- write output and some info
    if ( opt.check(OpenMesh::IO::Options::Binary) )
    {          
      std::cout << "  "
                << OpenMesh::IO::binary_size(mesh, ofname, opt)
                << std::endl;
    }
    if ( opt.vertex_has_normal() )
      std::cout << "  with vertex normals" << std::endl;
    if ( opt.vertex_has_color() )
      std::cout << "  with vertex colors" << std::endl;
    if ( opt.vertex_has_texcoord() )
      std::cout << "  with vertex texcoord" << std::endl;
    if ( opt.face_has_normal() )
      std::cout << "  with face normals" << std::endl;
    if ( opt.face_has_color() )
      std::cout << "  with face colors" << std::endl;
    std::cout << "  wrote in " << timer.as_string() << std::endl;
    timer.reset();       
  }

  return 0;
}
