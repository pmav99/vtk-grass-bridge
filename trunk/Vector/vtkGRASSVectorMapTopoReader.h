
/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2009 by Soeren Gebbert, soerengebbert@googlemail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _vtkGRASSVectorMapTopoReader_h
#define	_vtkGRASSVectorMapTopoReader_h

#include <vtkGRASSVectorMapBase.h>
#include "vtkGRASSBridgeVectorWin32Header.h"

extern "C" {
#include <grass/gis.h>
#include <grass/Vect.h>
}

class vtkGRASSVectorFeaturePoints;
class vtkGRASSVectorFeatureCats;
class vtkGRASSVectorBBox;

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorMapTopoReader : public vtkGRASSVectorMapBase {
public:
    static vtkGRASSVectorMapTopoReader *New();
    vtkTypeRevisionMacro(vtkGRASSVectorMapTopoReader, vtkGRASSVectorMapBase);
    void PrintSelf(ostream& os, vtkIndent indent);

    /*!\brief Open the vector map read only
     *
     * \param name the name of the vector map
     * \return true or false
     * */
    virtual bool OpenMap(const char *name) {
        return this->OpenMapReadOnly(name);
    }

    /*! \brief Read the line at index of the vector map (only level 2)
     *
     *  \return 1 on success,  -1 on error
     * */
    virtual int ReadFeature(vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats, int index);

//    int GetAreaFromCentroid(int centroid);
//    int GetArea(vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats, int area);

    //!\brief Returns area of area without areas of isles.
    double GetAreaOfArea(int area) {
        if (this->Open)return Vect_get_area_area(&this->map, area);
        else return -1;
    }


    //!\brief Get the bounding box of the map
    void GetBoundingBox(vtkGRASSVectorBBox *box);

    virtual int GetNumberOfFeatures() {
        int nprimitives = 0;

        nprimitives += Vect_get_num_primitives(&this->map, GV_POINT);
        nprimitives += Vect_get_num_primitives(&this->map, GV_LINE);
        nprimitives += Vect_get_num_primitives(&this->map, GV_BOUNDARY);
        nprimitives += Vect_get_num_primitives(&this->map, GV_FACE);
        nprimitives += Vect_get_num_primitives(&this->map, GV_CENTROID);
        nprimitives += Vect_get_num_primitives(&this->map, GV_KERNEL);
        if (this->Open)return (int) nprimitives;
        else return -1;
    }

    virtual int GetNumberOfLines() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_LINE);
        else return -1;
    }

    virtual int GetNumberOfPoints() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_POINT);
        else return -1;
    }

    virtual int GetNumberOfAreas() {
        if (this->Open)return (int) Vect_get_num_areas(&this->map);
        else return -1;
    }

    virtual int GetNumberOfFaces() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_FACE);
        else return -1;
    }

    virtual int GetNumberOfKernels() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_KERNEL);
        else return -1;
    }

    virtual int GetNumberOfIsles() {
        if (this->Open)return (int) Vect_get_num_islands(&this->map);
        else return -1;
    }

    virtual int GetNumberOfNodes() {
        if (this->Open)return (int) Vect_get_num_nodes(&this->map);
        else return -1;
    }

    virtual int GetNumberOfBoundaries() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_BOUNDARY);
        else return -1;
    }

    virtual int GetNumberOfCentroids() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_CENTROID);
        else return -1;
    }

    //!\brief Return the number of database links

    virtual int GetNumberOfDBLinks() {
        if (this->Open)return (int) Vect_get_num_dblinks(&this->map);
        else return -1;
    }

protected:
    vtkGRASSVectorMapTopoReader();

    ~vtkGRASSVectorMapTopoReader() {
    };

    void SetVectorLevelToTopo() {
        this->SetVectorLevel(2);
    }

private:
    vtkGRASSVectorMapTopoReader(const vtkGRASSVectorMapTopoReader&); // Not implemented.
    void operator=(const vtkGRASSVectorMapTopoReader&); // Not implemented.
};

#endif
