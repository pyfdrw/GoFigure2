#include "vtkLSMReader.h"
#include <iostream>
#include <sstream>
#include <string.h>
#include "vtkMetaImageWriter.h"
#include "vtkImageAppendComponents.h"
#include "vtkIndent.h"
#include "vtkImageData.h"
using namespace std;

// This code extracts a specified channel at a given time-point from an LSM file
// and writes out as an mha file

int main(int argc,char* argv[])
{
  const unsigned int Dimension = 3;

  if ( argc < 4 )
  {
    std::cout << "Usage: ";
    std::cout << "(exe) FileName TimePoint DirCh1 DirCh2";
    std::cout << std::endl;
    return 0;
  }

  //read the LSM file - can be multiple channels and time-points:
  vtkLSMReader* reader=vtkLSMReader::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  // Get the number of channels
  unsigned int NumberOfChannels = reader->GetNumberOfChannels();

  // Get the number of time-points
  unsigned int NumberOfTimePoints = reader->GetNumberOfTimePoints();
  unsigned int timePoint = atoi( argv[2] );

  if ( timePoint > NumberOfTimePoints )
    {
    std::cout << "The selected time-point exceeds the last time-point in the acquisition" << std::endl;
    }
  // Print out the number of time points and channels
  std::cout << NumberOfTimePoints << std::endl;
  std::cout << NumberOfChannels << std::endl;

  // Get the name of the file without the path
  unsigned int j, len = strlen(argv[1]);
  for( unsigned int i = len-1; i >= 0; i-- )
  {
    if ( argv[1][i] == '/' )
    {
      j = i;
      break;
    }
  }
  j++;

  char* fname = new char[len-j+1];
  for( unsigned int i = 0; i < len-j+1; i++ )
    fname[i]  = argv[1][j+i];

  vtkMetaImageWriter * writer = vtkMetaImageWriter::New();
  writer->SetFileDimensionality( Dimension );

  // Select the specified time-point and channel for write-out
  for( unsigned int channel = 0; channel < NumberOfChannels; channel++ )
    {
    reader->SetUpdateTimePoint( timePoint );
    reader->SetUpdateChannel( channel );
    reader->Update();

    // Determine the name of the file
    std::stringstream namebuffer;
    namebuffer << argv[3 + channel];
    namebuffer << fname;
    namebuffer << "_T_"  << timePoint;
    namebuffer << "_C_"  << channel;
    namebuffer << ".mha";
    std::cout << namebuffer.str().c_str() << std::endl;

    // Write-out
    writer->SetFileName( namebuffer.str().c_str() );
    writer->SetInputConnection( reader->GetOutputPort( ) );
    writer->Write();
    }

  return EXIT_SUCCESS;
}