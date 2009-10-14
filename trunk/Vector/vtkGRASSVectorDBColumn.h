
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
 * \brief This class provides read/write access to a database tables column of a vector map
 *
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Oct. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef _vtkGRASSVectorDBColumn_h
#define	_vtkGRASSVectorDBColumn_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeVectorWin32Header.h"
#include "vtkGRASSVectorMapBase.h"
#include "vtkGRASSDefines.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
#include <grass/dbmi.h>
}

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorDBColumn : public vtkObjectGRASSErrorHandler {
public:

    //BTX
    friend class vtkGRASSVectorDBTable;
    //ETX

    static vtkGRASSVectorDBColumn *New();
    vtkTypeRevisionMacro(vtkGRASSVectorDBColumn, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);

    //! \brief Reset the column and fill with default values
    void Initialize();

    const char *GetDescription() {
        if (this->column)
            return db_get_column_description(this->column);
        return NULL;
    }

    int GetLength() {
        if (this->column)
            return db_get_column_length(this->column);
        return -1;
    }

    int GetHostType() {
        if (this->column)
            return db_get_column_host_type(this->column);
        return -1;
    }

    const char *GetName() {
        if (this->column)
            return db_get_column_name(this->column);
        return NULL;
    }

    int GetPrecision() {
        if (this->column)
            return db_get_column_precision(this->column);
        return -1;
    }

    int GetScale() {
        if (this->column)
            return db_get_column_scale(this->column);
        return -1;
    }

    int GetSelectPrivate() {
        if (this->column)
            return db_get_column_select_priv(this->column);
        return -1;
    }

    int GetSQLType() {
        if (this->column)
            return db_get_column_sqltype(this->column);
        return -1;
    }

    int GetUpdatePrivate() {
        if (this->column)
            return db_get_column_update_priv(this->column);
        return -1;
    }

    bool IsValueDouble() {
        if (this->column)
            if (db_get_column_sqltype(this->column) == DB_C_TYPE_DOUBLE)
                return true;
        return false;
    }

    bool IsValueInteger() {
        if (this->column)
            if (db_get_column_sqltype(this->column) == DB_C_TYPE_INT)
                return true;
        return false;
    }

    bool IsValueString() {
        if (this->column)
            if (db_get_column_sqltype(this->column) == DB_C_TYPE_STRING)
                return true;
        return false;
    }

    bool IsValueDatetime() {
        if (this->column)
            if (db_get_column_sqltype(this->column) == DB_C_TYPE_DATETIME)
                return true;
        return false;
    }

    int GetValueInteger() {
        if (this->column)
            return db_get_value_int(db_get_column_value(this->column));
        return 0;
    }

    double GetValueDouble() {
        if (this->column)
            return db_get_value_double(db_get_column_value(this->column));
        return 0;
    }

    const char *GetValueString() {
        if (this->column)
            return db_get_value_string(db_get_column_value(this->column));
        return NULL;
    }

    void SetValueInteger(int value) {
        if (this->column)
            db_set_value_int(db_get_column_value(this->column), value);
    }

    void SetValueDouble(double value) {
        if (this->column)
            db_set_value_double(db_get_column_value(this->column), value);
    }

    void SetValueString(const char* string) {
        if (this->column)
            db_set_value_string(db_get_column_value(this->column), string);
    }

    char *GetValueAsString() {
        if (this->column) {
            dbString *valstr = (dbString*) (calloc(1, sizeof (dbString)));
            db_init_string(valstr);
            db_convert_column_value_to_string(this->column, valstr);
            return db_get_string(valstr);
        }
        return NULL;
    }

    void SetHasDefinedDefaultValue() {
        if (this->column)
            db_set_column_has_defined_default_value(this->column);
    }

    void SetHasUndefinedDefaultValue() {
        if (this->column)
            db_set_column_has_undefined_default_value(this->column);
    }

    void SetNullAllowed() {
        if (this->column)
            db_set_column_null_allowed(this->column);
    }

    void SetUseDefaultValue() {
        if (this->column)
            db_set_column_use_default_value(this->column);
    }

    void SetSelectPrivateNotGranted() {
        if (this->column)
            db_set_column_select_priv_not_granted(this->column);
    }

    void SetSelectPrivateGranted() {
        if (this->column)
            db_set_column_select_priv_granted(this->column);
    }

    void SetDescription(const char*description) {
        if (this->column)
            db_set_column_description(this->column, description);
    }

    void SetHstType(int host_type) {
        if (this->column)
            db_set_column_host_type(this->column, host_type);
    }

    void SetLength(int length) {
        if (this->column)
            db_set_column_length(this->column, length);
    }

    void SetName(const char* name) {
        if (this->column)
            db_set_column_name(this->column, name);
    }

    void SetPrecision(int precision) {
        if (this->column)
            db_set_column_precision(this->column, precision);
    }

    void SetScale(int scale) {
        if (this->column)
            db_set_column_scale(this->column, scale);
    }

    void SetSQLType(int sql_type) {
        if (this->column)
            db_set_column_sqltype(this->column, sql_type);
    }

    //BTX
    dbColumn *GetPointer() {
        return this->column;
    }
    // ETX

protected:
    vtkGRASSVectorDBColumn();
    ~vtkGRASSVectorDBColumn();

    //BTX
    void SetColumn(dbColumn *column);
    dbColumn *column;
    //ETX

private:
    vtkGRASSVectorDBColumn(const vtkGRASSVectorDBColumn&); // Not implemented.
    void operator=(const vtkGRASSVectorDBColumn&); // Not implemented.
};

#endif
