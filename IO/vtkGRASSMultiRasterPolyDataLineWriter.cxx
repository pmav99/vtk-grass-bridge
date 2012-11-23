/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2012 by Soeren Gebbert, soerengebbert@googlemail.com
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

#include <string>

#include <vtkDataSetAttributes.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkIdList.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkShortArray.h>
#include "vtkDCELL.h"
#include "vtkGRASSRasterMemoryMap.h"
#include "vtkGRASSMultiRasterPolyDataLineWriter.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSMessagingInterface.h"

extern "C" {
#include <string.h>
}

vtkCxxRevisionMacro(vtkGRASSMultiRasterPolyDataLineWriter, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkGRASSMultiRasterPolyDataLineWriter);

//----------------------------------------------------------------------------

vtkGRASSMultiRasterPolyDataLineWriter::vtkGRASSMultiRasterPolyDataLineWriter() {
	this->RasterBaseName = NULL;
	this->SetNumberOfInputPorts(1);
	this->SetNumberOfOutputPorts(0);
}

//----------------------------------------------------------------------------

vtkGRASSMultiRasterPolyDataLineWriter::~vtkGRASSMultiRasterPolyDataLineWriter() {
	if (this->RasterBaseName)
		delete[] this->RasterBaseName;
}

//----------------------------------------------------------------------------

void vtkGRASSMultiRasterPolyDataLineWriter::PrintSelf(ostream& os,
		vtkIndent indent) {
	this->Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------------

int vtkGRASSMultiRasterPolyDataLineWriter::RequestData(vtkInformation * vtkNotUsed(request),
                                    vtkInformationVector **inputVector,
                                    vtkInformationVector *outputVector)
{

	vtkIdType n;
	int layer;
	int nArrays, aCount;
	int rows, row, cols, col;
	double x, y;
	double bounds[6];
	int nLayers, lCount;
	VGB_CREATE(vtkDCELL, dvalue);
	char message[1024];

	VGB_CREATE(vtkGRASSMessagingInterface, talk);

	if (this->RasterBaseName == NULL) {
		vtkErrorMacro( << "The DataName must be specified");
		return -1;
	}

	vtkPolyData* input = vtkPolyData::GetData(inputVector[0]);

	if (!input->GetCellData()->HasArray("Layer")) {
		vtkErrorMacro(
				"Array with layer information is missing in input.");
		return -1;
	}

	nArrays = input->GetCellData()->GetNumberOfArrays();

	// Number of Layers
	nLayers = (int)input->GetCellData()->GetArray("Layer")->GetRange()[1];
	vtkDataArray *layerArray = input->GetCellData()->GetArray("Layer");

	for(lCount = 0; lCount < nLayers; lCount++) {
		layer = lCount + 1;

		G_snprintf(message, 1024, "Processing layer %i of %i", lCount + 1,
				nLayers);
		talk->Message(message);

		for(aCount = 0; aCount < nArrays; aCount++) {

			double value;
			char mapName[1024];
			vtkDataArray *array = input->GetCellData()->GetArray(aCount);
			const char *arrayName = array->GetName();

			// Check if we have the layer array
			if(strcmp(arrayName, "Layer") == 0)
				continue;

			G_snprintf(message, 1024, "(%i of %i) Processing array %s", aCount + 1,
					nArrays, arrayName);
			talk->Message(message);

			VGB_CREATE(vtkGRASSRasterMapWriter, RasterMap);
			VGB_CREATE(vtkGRASSRasterMemoryMap, mmap);
			VGB_CREATE(vtkDCELL, dcell);

			G_snprintf(mapName, 1024, "%s_%s_%i", this->RasterBaseName,
					arrayName, layer);

			RasterMap->SetMapTypeToDCELL();
			RasterMap->UseCurrentRegion();
			if(!RasterMap->OpenMap(mapName))
				return -1;

			rows = RasterMap->GetNumberOfRows();
			cols = RasterMap->GetNumberOfCols();

			if(!mmap->Allocate(rows, cols, RasterMap->GetMapType()))
				return -1;
			mmap->SetToNull();

			for(n = 0; n < input->GetNumberOfCells(); n++) {

				talk->Percent(n, input->GetNumberOfCells(), 2);

				if((int)layerArray->GetTuple1(n) != layer)
					continue;

				input->GetCell(n)->GetBounds(bounds);
				x = (bounds[0] + bounds[1])/2.0;
				y = (bounds[2] + bounds[3])/2.0;

				row = RasterMap->NorthingToRow(y);
				col = RasterMap->EastingToColumn(x);

				// Write only coordinates that are in bounds
				if(col < 0 or col > cols - 1)
					continue;
				if(row < 0 or row > rows - 1)
					continue;

				//std::cout << "Write value: " << value << " at position [" <<
				//		row << "][" << col << "]" << std::endl;

				value = array->GetTuple1(n);
				dcell->Value = value;
				mmap->SetDCELLValue(row, col, dcell);
			}
			if(!RasterMap->WriteMemoryMap(mmap))
				return -1;
			if(!RasterMap->CloseMap())
				return -1;
			talk->Percent(1, 1, 1);
		}
	}
	return 1;
}
