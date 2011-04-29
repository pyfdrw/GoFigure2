
/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009-11

 Copyright (c) 2009-11, President and Fellows of Harvard College.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 Neither the name of the  President and Fellows of Harvard College
 nor the names of its contributors may be used to endorse or promote
 products derived from this software without specific prior written
 permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include "MegaImageProcessor.h"

// external library include
#include "vtkLookupTable.h"
#include "vtkMath.h"
#include "vtkImageMapToColors.h"
#include "vtkImageBlend.h"

// debug
#include "VisualizePolydataHelper.h"

//--------------------------------------------------------------------------
MegaImageProcessor::MegaImageProcessor():m_MegaImageReader(NULL), m_Output(NULL)
{
  std::cout << "simple constructor" << std::endl;
  m_MegaImageReader = itk::MegaCaptureReader::New();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
MegaImageProcessor::MegaImageProcessor(itk::MegaCaptureReader::Pointer iReader):
  m_MegaImageReader(iReader)
{
    std::cout << "overloaded constructor" << std::endl;
  //Create the new MegaImageStructure
  // with the first time point and all the channels
  unsigned int time = m_MegaImageReader->GetMinTimePoint();
  setTimePoint(time);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
MegaImageProcessor::MegaImageProcessor(const MegaImageProcessor & iE):
  m_MegaImageReader(iE.m_MegaImageReader),
  m_MegaImageContainer(iE.m_MegaImageContainer),
  m_Output(iE.m_Output)
{
    std::cout << "copy constructor" << std::endl;
  //Create the new MegaImageStructure
  // with the first time point and all the channels
  unsigned int time = m_MegaImageReader->GetMinTimePoint();
  setTimePoint(time);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
MegaImageProcessor::
~MegaImageProcessor()
{
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
MegaImageProcessor::
setMegaReader(itk::MegaCaptureReader::Pointer iReader)
{
  m_MegaImageReader = iReader;

  //Create the new MegaImageStructure
  // with the first time point and all the channels
  unsigned int time = m_MegaImageReader->GetMinTimePoint();
  std::cout << "min time: " << time << std::endl;
  setTimePoint(time);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkSmartPointer<vtkLookupTable>
MegaImageProcessor::
createLUT(const double& iRed, const double& iGreen, const double& iBlue,
          const double& iAlpha)
{
  std::cout<< " R: " << iRed
           << " G: " << iGreen
           << " B: " << iBlue
           << std::endl;
  vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
  double* HSV = vtkMath::RGBToHSV(iRed,iGreen,iBlue);
  lut->SetAlpha(iAlpha);
  lut->SetHueRange(HSV[0], HSV[0]);
  lut->SetSaturationRange(1, 1);
  lut->SetValueRange(0, 1);
  lut->Build();
  return lut;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
MegaImageProcessor::
setLookupTable(vtkSmartPointer<vtkLookupTable> iLUT,
                    const unsigned int& iChannel,
                    const unsigned int& iTime)
{
  MegaImageStructureMultiIndexContainer::index<Channel>::type::iterator it =
      m_MegaImageContainer.get< Channel >().find(iChannel);

  while(it!=m_MegaImageContainer.get< Channel >().end())
    {
    if(it->Time==iTime)
      {
      m_MegaImageContainer.get< Channel >().modify( it , set_lut(iLUT) );
      break;
      }
    ++it;
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkSmartPointer<vtkLookupTable>
MegaImageProcessor::
getLookuptable(const unsigned int& iChannel, const unsigned int& iTime) const
{
  MegaImageStructureMultiIndexContainer::index<Channel>::type::iterator it =
      m_MegaImageContainer.get< Channel >().find(iChannel);

  while(it!=m_MegaImageContainer.get< Channel >().end())
    {
    if(it->Time==iTime)
      {
      return it->LUT;
      }
    ++it;
    }
  return NULL;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkSmartPointer<vtkImageData>
MegaImageProcessor::
colorImage(vtkSmartPointer<vtkImageData> iImage,
           vtkSmartPointer<vtkLookupTable> iLUT)
{
  vtkSmartPointer<vtkImageMapToColors> coloredImage =
      vtkSmartPointer<vtkImageMapToColors>::New();
  coloredImage->SetInput( iImage);
  coloredImage->SetLookupTable(iLUT);
  coloredImage->Update();

  return coloredImage->GetOutput();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkSmartPointer<vtkImageData>
MegaImageProcessor::
getImage(const unsigned int& iTime, const unsigned int& iChannel)
{
  MegaImageStructureMultiIndexContainer::index<Channel>::type::iterator it =
      m_MegaImageContainer.get< Channel >().find(iChannel);

  while(it!=m_MegaImageContainer.get< Channel >().end())
    {
    if(it->Time==iTime)
      {
      return colorImage(it->Image, it->LUT);
      }
    ++it;
    }
  return NULL;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkSmartPointer<vtkImageData>
MegaImageProcessor::
getTimeAllChannels(const unsigned int& iTime)
{
  vtkSmartPointer<vtkImageBlend> blendedImage =
      vtkSmartPointer<vtkImageBlend>::New();

  MegaImageStructureMultiIndexContainer::index<Time>::type::iterator it =
      m_MegaImageContainer.get< Time >().find(iTime);

  vtkIdType i(0);

  while(it!=m_MegaImageContainer.get< Time >().end())
    {
     ShowImage(colorImage(it->Image, it->LUT));
    blendedImage->AddInput(colorImage(it->Image, it->LUT));
    // to be enhanced
    blendedImage->SetOpacity(i, 0.5);
    ++i;
    ++it;
    }
  blendedImage->Update();
  return blendedImage->GetOutput();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkSmartPointer<vtkImageData>
MegaImageProcessor::
getChannelAllTimes(const unsigned int& iChannel)
{
  vtkSmartPointer<vtkImageBlend> blendedImage =
      vtkSmartPointer<vtkImageBlend>::New();

  MegaImageStructureMultiIndexContainer::index<Channel>::type::iterator it =
      m_MegaImageContainer.get< Channel >().find(iChannel);

  vtkIdType i(0);

  while(it!=m_MegaImageContainer.get< Channel >().end())
    {
    blendedImage->AddInput(colorImage(it->Image, it->LUT));

   // might not be requiered - to be checked
    blendedImage->SetOpacity(i, 1);
    ++i;
    ++it;
    }
  blendedImage->Update();
  return blendedImage->GetOutput();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
MegaImageProcessor::
setTimePoint(const unsigned int& iTime)
{
  //check if time point exists
  if(iTime >= m_MegaImageReader->GetMinTimePoint() &&
      iTime <= m_MegaImageReader->GetMaxTimePoint())
    {
    m_MegaImageReader->SetTimePoint(iTime);
    m_MegaImageContainer.clear();
    }
  else
    {
    return;
    }

  // update the container
  // Get Number of channels from reader
  unsigned int min_ch = m_MegaImageReader->GetMinChannel();
  unsigned int max_ch = m_MegaImageReader->GetMaxChannel();
  int numberOfChannels = max_ch - min_ch;

  std::cout << "fill container: "<< std::endl;
  std::cout << "container size: "<< m_MegaImageContainer.size() << std::endl;
  std::cout << "number of channels: " << numberOfChannels << std::endl;

  m_MegaImageReader->Update();

  while(numberOfChannels>=0)
    {
        std::cout << "time: " << iTime  << std::endl;
        std::cout << "channel: " << numberOfChannels  << std::endl;
    // Get useful information from the reader
    // Get Image
    vtkSmartPointer<vtkImageData> image =
        m_MegaImageReader->GetOutput(numberOfChannels);

    // Get Color
    // Create LUT
    // generates random colors as of now
    double random1 = rand() % 255;
    double value1 = random1/255;

    double random2 = rand() % 255;
    double value2 = random2/255;

    double random3= rand() % 255;
    double value3 = random3/255;

    vtkSmartPointer<vtkLookupTable> lut = createLUT(value1,
                                                    value2,
                                                    value3,
                                                    1);
    // set up range
    lut->SetRange(image->GetScalarRange());
    lut->Build();

    // Update the MegaImageStructure
    // image, LUT, channel, time point
    m_MegaImageContainer.insert(MegaImageStructure(iTime,
                                                   numberOfChannels,
                                                   lut,
                                                   image));// Image
    std::cout << "container after insert: "<< m_MegaImageContainer.size() << std::endl;

    --numberOfChannels;
    }
    std::cout << "container final size: "<< m_MegaImageContainer.size() << std::endl;
}
//--------------------------------------------------------------------------
