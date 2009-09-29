
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
 * \brief This class provides write access to a new vector map
 *
 * This class provides an interface to write grass vector maps.
 *
 * \TODO Implement database support
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */
#ifndef _vtkGRASSVectorMapWriter_h
#define	_vtkGRASSVectorMapWriter_h

#include <vtkGRASSVectorMapTopoReader.h>
#include "vtkGRASSBridgeVectorWin32Header.h"
#include "vtkGRASSVectorFeaturePoints.h"
#include "vtkGRASSVectorFeatureCats.h"

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorMapWriter : public vtkGRASSVectorMapTopoReader {
public:
    static vtkGRASSVectorMapWriter *New();
    vtkTypeRevisionMacro(vtkGRASSVectorMapWriter, vtkGRASSVectorMapTopoReader);


    /*!\brief Open a new vector map to write
     *
     * \param name of the vector map
     * \param with_z open in 2d or 3d mode
     * \return true or false
     * */
    virtual bool OpenMap(const char *name, int with_z = 0);

    /*! \brief close vector map
     *
     *  \param build_topo 1 - build topology, 0 do not build topology
     *  \return true for success, false for failure
     *
     * */
    virtual bool CloseMap(int build_topo = 0);
     /*!
       \brief Writes new feature to the end of file (table)

       \param type feature type
       \param points feature geometry
       \param cats feature categories

       \return new feature id (level 2) or offset into file where the feature starts (level 1)
     */

    virtual int WriteFeature(int type, vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats){
         if(this->Open) return Vect_write_line(&this->map, type, points->GetPointer(), cats->GetPointer());
         else return -1;
     }

    /*!
       \brief Delete feature

       Vector map must be opened on topo level 2.

       \param line feature id

       \return 0 on success
       \return -1 on error
     */
    int DeleteFeature(int line) {
        if (this->Open)return Vect_delete_line(&this->map, line);
        else return -1;
    }
    /*!
       \brief Rewrites feature info at the given offset.

       The number of points or cats or type may change. If necessary, the
       old feature is deleted and new is written.

       \param line feature id
       \param type feature type
       \param points feature geometry
       \param cats feature categories

       \return new feature id
       \return -1 on error
     */
    int RewriteFeature(int line, int type, vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats){
         if(this->Open) return Vect_rewrite_line(&this->map, line, type, points->GetPointer(), cats->GetPointer());
         else return -1;
    }

    /*!
       \brief Restore previously deleted feature

       Vector map must be opened on topo level 2.

       \param line feature id to be deleted

       \return 0 on success
       \return -1 on error
     */
    int RestoreFeature(int line, off_t offset){
        if (this->Open)return Vect_restore_line(&this->map, line, offset);
        else return -1;
    }

    //!\brief Set the name of the organisation
    int SetOrganisation(const char *org) {
        if (this->Open)return Vect_set_organization(&this->map, org);
        else return -1;
    }
    //!\brief Set date of creation as string
    int SetCreationDate(const char *date) {
        if (this->Open)return Vect_set_date(&this->map, date);
        else return -1;
    }
    //!\brief Set the name of the creator
    int SetPerson(const char* person) {
        if (this->Open)return Vect_set_person(&this->map, person);
        else return -1;
    }
    //!\brief Set the title of the map
    int SetTitle(const char *title) {
        if (this->Open)return Vect_set_map_name(&this->map, title);
        else return -1;
    }
    //!\brief Set the scale
    int SetScale(int scale) {
        if (this->Open)return Vect_set_scale(&this->map, scale);
        else return -1;
    }
    //!\brief Set the map threshold
    int SetThreshold(double thres) {
        if (this->Open)return Vect_set_thresh(&this->map, thres);
        else return -1;
    }

    //! \brief Build the topology of the vector map
    virtual int Build(){
        if (this->Open)return Vect_build(&this->map);
        else return -1;
    }

    //!\brief Set open new vector map without topology support
    void SetVectorLevelToNoTopo() {
        this->SetVectorLevel(1);
    }
    //!\brief Set open new vector map with topology support
    void SetVectorLevelToTopo() {
        this->SetVectorLevel(2);
    }


protected:
    vtkGRASSVectorMapWriter();
    ~vtkGRASSVectorMapWriter(){};

    vtkSetMacro(Open3D, int);

    int Open3D;


private:
    vtkGRASSVectorMapWriter(const vtkGRASSVectorMapWriter&); // Not implemented.
    void operator=(const vtkGRASSVectorMapWriter&); // Not implemented.
};

#endif
