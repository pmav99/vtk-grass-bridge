
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

/*!
 *
 * \TODO add documentation for class and member variables/functions
 *
 * */
#ifndef _vtkGRASSVectorMapTopoReader_h
#define	_vtkGRASSVectorMapTopoReader_h

#include <vtkGRASSVectorMapBase.h>
#include "vtkGRASSBridgeVectorWin32Header.h"
#include "vtkGRASSVectorFeaturePoints.h"
#include "vtkGRASSVectorBBox.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}

class vtkGRASSVectorFeaturePoints;
class vtkGRASSVectorFeatureCats;
class vtkGRASSVectorBBox;
class vtkIntArray;

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

    /*************** AREA FUNCTIONS *********************************/
    int GetAreaFromCentroid(int centroid);
    int GetAreaPoints(vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats, int area);
    int GetIslePoints(vtkGRASSVectorFeaturePoints *points, int isle) {
        if (this->Open) return Vect_get_isle_points (&this->map, isle, points->GetPointer());
        else return -1;
    }
    int GetCentroidFromArea(int isle ){
        if (this->Open) return Vect_get_area_centroid (&this->map, isle);
        else return -1;
    }
    int GetAreaBoundaries(int area, vtkIntArray *boundaryids);
    int GetIsleBoundaries(int area, vtkIntArray *boundaryids);
    int GetNumberOfAreaIsles(int area){
        if (this->Open) return Vect_get_area_num_isles (&this->map, area);
        else return -1;
    }
    int GetAreaIsle(int area, int isle){
        if (this->Open) return Vect_get_area_isle (&this->map, area, isle);
        else return -1;
    }
    int GetIsleArea(int isle){
        if (this->Open) return Vect_get_isle_area (&this->map, isle);
        else return -1;
    }
    int IsPointInArea(int area, double x, double y){
        if (this->Open) return Vect_point_in_area (&this->map, area, x, y);
        else return -1;
    }
    //!\brief Returns area of area without areas of isles.
    double GetAreaOfArea(int area) {
        if (this->Open)return Vect_get_area_area(&this->map, area);
        else return -1;
    }
    //!\brief Returns the perimeter of area
    double GetPerimeterOfArea(vtkGRASSVectorFeaturePoints *areapoints) {
        if (this->Open)return Vect_area_perimeter(areapoints->GetPointer());
        else return -1;
    }
        //!\brief Returns area of area without areas of isles.
    double GetAreaBoundingBox(int area, vtkGRASSVectorBBox *box) {
        if (this->Open)return Vect_get_area_box(&this->map, area, box->GetPointer());
        else return -1;
    }
    int FindArea(double x, double y){
        if (this->Open)return Vect_find_area(&this->map, x, y);
        else return -1;
    }
    int FindIsle(double x, double y){
        if (this->Open)return Vect_find_island(&this->map, x, y);
        else return -1;
    }

    /***************************** FIND FUNCTIONS *****************************/

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

    //!\brief Return the number of holes
    virtual int GetNumberOfHoles() {
        if (this->Open)return (int) Vect_get_num_holes(&this->map);
        else return -1;
    }

    //!\brief Return the number of database links
    virtual int GetNumberOfDBLinks() {
        if (this->Open)return (int) Vect_get_num_dblinks(&this->map);
        else return -1;
    }
    virtual int SelectLinesByBox(vtkGRASSVectorBBox *box, int type, vtkIntArray *ids);
    virtual int SelectAreasByBox(vtkGRASSVectorBBox *box, vtkIntArray *ids);
    virtual int SelectIslesByBox(vtkGRASSVectorBBox *box, vtkIntArray *ids);
    virtual int SelectNodesByBox(vtkGRASSVectorBBox *box, vtkIntArray *ids);
/*
  int   Vect_select_lines_by_box (struct Map_info *Map, const struct bound_box *Box, int type, struct ilist *list)
     Select lines by box.
  int   Vect_select_areas_by_box (struct Map_info *Map, const struct bound_box *Box, struct ilist *list)
     Select areas by box.
  int   Vect_select_isles_by_box (struct Map_info *Map, const struct bound_box *Box, struct ilist *list)
     Select isles by box.
  int   Vect_select_nodes_by_box (struct Map_info *Map, const struct bound_box *Box, struct ilist *list)
     Select nodes by box.
*/
    int DeepCopy(vtkGRASSVectorMapTopoReader *src){
        if(this->Open && src->IsOpen()) return Vect_copy_map_lines(src->GetPointer(), this->GetPointer());
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
