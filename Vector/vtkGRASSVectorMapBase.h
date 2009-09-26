
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

#ifndef _vtkGRASSVectorMapBase_h
#define	_vtkGRASSVectorMapBase_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeVectorWin32Header.h"

class vtkGRASSVectorFeaturePoints;
class vtkGRASSVectorFeatureCats;
class vtkGRASSVectorBBox;

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorMapBase : public vtkObjectGRASSErrorHandler {
public:
    static vtkGRASSVectorMapBase *New();
    vtkTypeRevisionMacro(vtkGRASSVectorMapBase, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);


    /*! \brief Read the next line of the vector map
     *
     *  \return position in file, -1 on error
     * */
    virtual int ReadNextFeature(vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats);

    /*! \brief Rewind the vector map
     *
     *  \return 1 on success,  -1 on error
     * */
    virtual int RewindMap() {
        return Vect_rewind(&this->map);
    }

    /*! \brief Close the map and free any memory
     *
     * */
    virtual bool CloseMap();

    //! \brief Returns thr opening state
    //! \return true for open, false for not open

    virtual bool IsOpen() {
        return this->Open;
    }

    //!\brief Return the name of the organisation

    const char *GetOrganisation() {
        if (this->Open)return Vect_get_organization(&this->map);
        else return NULL;
    }
    //!\brief Return the name of the date of creation as string of the map

    const char *GetCreationDate() {
        if (this->Open)return Vect_get_date(&this->map);
        else return NULL;
    }
    //!\brief Return the name of the creator

    const char *GetPerson() {
        if (this->Open)return Vect_get_person(&this->map);
        else return NULL;
    }
    //!\brief Return the name of the map

    const char *GetTitle() {
        if (this->Open)return Vect_get_map_name(&this->map);
        else return NULL;
    }
    //!\brief Return the full name of the map (map@mapset)

    const char *GetFullName() {
        if (this->Open)return Vect_get_full_name(&this->map);
        else return NULL;
    }
    //!\brief Return the mapset of the map

    const char *GetMapset() {
        if (this->Open)return Vect_get_mapset(&this->map);
        else return NULL;
    }
    //!\brief Return the name of the date of the map

    const char *GetMapDate() {
        if (this->Open)return Vect_get_map_date(&this->map);
        else return NULL;
    }
    //!\brief Return the name of the scale

    int GetScale() {
        if (this->Open)return Vect_get_scale(&this->map);
        else return 0;
    }
    //!\brief Return the name of the map comment

    const char *GetComment() {
        if (this->Open)return Vect_get_comment(&this->map);
        else return NULL;
    }
    //!\brief Return the name of the map zone

    int GetZone() {
        if (this->Open)return Vect_get_zone(&this->map);
        else return -1;
    }
    //!\brief Return the name of the map threshold

    double GetThreshold() {
        if (this->Open)return Vect_get_thresh(&this->map);
        else return -1;
    }
    //!\brief Return the projection number of the map

    int GetProjection() {
        if (this->Open)return Vect_get_proj(&this->map);
        else return -1;
    }
    //!\brief Return the projection name of the map

    const char *GetProjectionName() {
        if (this->Open)return Vect_get_proj_name(&this->map);
        else return NULL;
    }

    /*!
       \brief Check if vector map is 3D (with z)

       \return 1 map is 3D
       \return 0 map is not 3D
     * \return -1 map not open
     */

    virtual int Is3d() {
        if (this->Open)return Vect_is_3d(&this->map);
        else return -1;
    }

    //!\brief compute the total number of points used by this map. This may be very time consuming.
    virtual int GetTotalNumberOfPoints();

    //BTX
    virtual struct Map_info *GetPointer() {
        return &this->map;
    }
    //ETX

protected:
    vtkGRASSVectorMapBase();
    ~vtkGRASSVectorMapBase();

    int VectorLevel;
    long TotalNumberOfPoints;
    bool Initiated;
    bool Open;
    char *VectorName;

    //BTX
    struct Map_info map;
    //ETX

    //!\brief Set the level hte vector map should be opened, default is 2
    //TODO: describe the levels
    vtkSetMacro(VectorLevel, int);
    vtkSetStringMacro(VectorName);
    int CheckBeforOpen(vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats);
    
    //!\brief Set open vector map without topology support

    void SetVectorLevelToNoTopo() {
        this->SetVectorLevel(1);
    }

    /*!\brief Open the vector map read only
     *
     * \param name the name of the vector map
     * \return true or false
     * */
    virtual bool OpenMapReadOnly(const char *name);

private:
    vtkGRASSVectorMapBase(const vtkGRASSVectorMapBase&); // Not implemented.
    void operator=(const vtkGRASSVectorMapBase&); // Not implemented.
};

#endif
