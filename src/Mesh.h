#pragma once

#include <string>
#include <vector>
#include <tr1/memory>

#include "EigenTypedef.h"

class MeshAttributes;
class MeshConnectivity;
class MeshFactory;
class MeshGeometry;

/**
 * A barebone mesh structure for surface and volumetric meshes.
 *
 * This class is composed of 3 loosely coupled concepts:
 * 1. Geometry    : vertices, faces, voxels.
 * 2. Connectivity: who is connected to whom.
 * 3. Attributes  : values associated with each vertex/face/voxel.
 *
 * This design allows user to only deal with the Mesh object and remain ignorant
 * about the structure/logic underneath the facade.
 *
 * Limitation:
 * 1. only triangles are supported for now.
 */
class Mesh {
    public:
        // Use MeshFactory class to create mesh object.
        friend class MeshFactory;

    protected:
        /**
         * Constructor to create an empty mesh.
         * This constructor is only accessible by friends.
         */
        Mesh();

        // Disable evil copy constructors.
        // No implementation provided on purpose.
        Mesh(Mesh& other);
        Mesh& operator=(Mesh& other);

    public:
        virtual ~Mesh() {}

    public:
        // Geometry access
        size_t get_dim() { return 3; }
        size_t get_num_vertices() {
            return get_vertices().size() / 3;
        }
        size_t get_num_faces() {
            return get_faces().size() / 3;
        }
        size_t get_num_voxels() {
            if (get_vertex_per_voxel() == 0)
                return 0;
            else 
                return get_voxels().size() / get_vertex_per_voxel();
        }

        Vector3F get_vertex(size_t i);
        VectorI get_face(size_t i);
        VectorI  get_voxel(size_t i);

        VectorF& get_vertices();
        VectorI& get_faces();
        VectorI& get_voxels();

        int get_vertex_per_face() const;
        int get_vertex_per_voxel() const;

        // Connectivity access
        VectorI get_vertex_adjacent_vertices(size_t vi) const;
        VectorI get_vertex_adjacent_faces(size_t vi) const;
        VectorI get_vertex_adjacent_voxels(size_t vi) const;

        VectorI get_face_adjacent_faces(size_t fi) const;
        VectorI get_face_adjacent_voxels(size_t fi) const;

        VectorI get_voxel_adjacent_faces(size_t Vi) const;
        VectorI get_voxel_adjacent_voxels(size_t Vi) const;

        // Attribute access
        bool has_attribute(const std::string& attr_name);
        void add_attribute(const std::string& attr_name);
        void remove_attribute(const std::string& attr_name);
        VectorF& get_attribute(const std::string& attr_name);
        void set_attribute(const std::string& attr_name, VectorF& attr_value);
        std::vector<std::string> get_attribute_names() const;

    public:
        typedef std::tr1::shared_ptr<MeshGeometry>     GeometryPtr;
        typedef std::tr1::shared_ptr<MeshConnectivity> ConnectivityPtr;
        typedef std::tr1::shared_ptr<MeshAttributes>   AttributesPtr;

    protected:
        GeometryPtr     get_geometry() { return m_geometry; }
        ConnectivityPtr get_connectivity() { return m_connectivity; }
        AttributesPtr   get_attributes() {
            return m_attributes; }

        void set_geometry(GeometryPtr geometry);
        void set_connectivity(ConnectivityPtr connectivity);
        void set_attributes(AttributesPtr attributes);

    protected:
        GeometryPtr     m_geometry;
        ConnectivityPtr m_connectivity;
        AttributesPtr   m_attributes;
};
