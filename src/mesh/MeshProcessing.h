/*
 * MeshProcessing.h
 * @author : Team OpenSculpt
 *
 * This file gives the declaration of the class MeshProcessing.
 * A MeshProcessing is an object with many process usable on a Mesh.
 *
 */

#ifndef MESHPROCESSING_H
#define MESHPROCESSING_H

#include "Mesh.h"

class MeshProcessing {

    // ================== PUBLIC CONSTRUCTORS ==================
public:
    MeshProcessing();
    ~MeshProcessing();
    // =========================================================


    // ================== PUBLIC MEMBERS ==================
public:
    // Subdivide or Decimate the entire Mesh
    static void subdivide(Mesh *mesh);
    static void decimate(Mesh *mesh);

    // Subdivide or Decimate only half-edges wich break the criterion
    static bool subdivideAuto(Mesh *mesh, float maxEdgeLength);
    static bool decimateAuto(Mesh *mesh, float minEdgeLength);

    // Scale the entire Mesh along the three axis separately
    static void scale(Mesh *mesh, QVector3D coef);
    // ====================================================
};

#endif // MESHPROCESSING_H
