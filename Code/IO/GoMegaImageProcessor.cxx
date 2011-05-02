
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

#include "GoMegaImageProcessor.h"

// external library include
#include "vtkLookupTable.h"
#include "vtkMath.h"
#include "vtkImageMapToColors.h"
#include "vtkImageBlend.h"

// debug
#include "VisualizePolydataHelper.h"

//--------------------------------------------------------------------------
GoMegaImageProcessor::GoMegaImageProcessor():m_MegaImageReader(NULL), m_Output(NULL),
  m_BoundsTime(NULL), m_BoundsChannel(NULL), m_Extent(NULL), m_DopplerStep(1),
  m_NumberOfImages(3)
{
  m_MegaImageReader = itk::MegaCaptureReader::New();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
GoMegaImageProcessor::GoMegaImageProcessor(itk::MegaCaptureReader::Pointer iReader):
  m_MegaImageReader(iReader), m_Output(NULL), m_BoundsTime(NULL),
  m_BoundsChannel(NULL), m_Extent(NULL), m_DopplerStep(1), m_NumberOfImages(3)
{
  //Create the new MegaImageStructure
  // with the first time point and all the channels
  unsigned int time = m_MegaImageReader->GetMinTimePoint();
  setTimePoint(time);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
GoMegaImageProcessor::GoMegaImageProcessor(const GoMegaImageProcessor & iE):
  m_MegaImageReader(iE.m_MegaImageReader),
  m_MegaImageContainer(iE.m_MegaImageContainer), m_Output(iE.m_Output),
  m_BoundsTime(iE.m_BoundsTime), m_BoundsChannel(iE.m_BoundsChannel),
  m_Extent(iE.m_Extent), m_DopplerStep(iE.m_DopplerStep),
  m_NumberOfImages(iE.m_NumberOfImages)
{
  unsigned int time = m_MegaImageReader->GetMinTimePoint();
  setTimePoint(time);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
GoMegaImageProcessor::
~GoMegaImageProcessor()
{
  if(m_BoundsTime)
    {
    delete[] m_BoundsTime;
    m_BoundsTime = NULL;
    }

  if(m_BoundsChannel)
    {
    delete[] m_BoundsChannel;
    m_BoundsChannel = NULL;
    }

  if(m_Extent)
    {
    delete[] m_Extent;
    m_Extent = NULL;
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
GoMegaImageProcessor::
setMegaReader(itk::MegaCaptureReader::Pointer iReader)
{
  m_MegaImageReader = iReader;

  std::cout << "init parameters" << std::endl;

  // update general parameters
  //--------------------
  // todo Nicolas- Create a method for that...
  m_BoundsTime = new unsigned int[2];
  m_BoundsTime[0] = m_MegaImageReader->GetMinTimePoint();
  m_BoundsTime[1] = m_MegaImageReader->GetMaxTimePoint();
  m_BoundsChannel = new unsigned int[2];
  m_BoundsChannel[0] = m_MegaImageReader->GetMinChannel();
  m_BoundsChannel[1] = m_MegaImageReader->GetMaxChannel();
  m_Extent = new int[6];
  (m_MegaImageReader->GetImage(m_BoundsChannel[0], m_BoundsTime[0]))->
      GetExtent(m_Extent);

  m_TimeInterval = m_MegaImageReader->GetTimeInterval();

  //--------------------

  unsigned int time = m_MegaImageReader->GetMinTimePoint();

  setTimePoint(time);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkSmartPointer<vtkLookupTable>
GoMegaImageProcessor::
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
GoMegaImageProcessor::
setLookupTable(vtkSmartPointer<vtkLookupTable> iLUT,
                    const unsigned int& iChannel,
                    const unsigned int& iTime)
{
  GoMegaImageStructureMultiIndexContainer::index<Channel>::type::iterator it =
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
GoMegaImageProcessor::
getLookuptable(const unsigned int& iChannel, const unsigned int& iTime) const
{
  GoMegaImageStructureMultiIndexContainer::index<Channel>::type::iterator it =
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
GoMegaImageProcessor::
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
GoMegaImageProcessor::
getImage(const unsigned int& iTime, const unsigned int& iChannel)
{
  GoMegaImageStructureMultiIndexContainer::index<Channel>::type::iterator it =
      m_MegaImageContainer.get< Channel >().find(iChannel);

  while(it!=m_MegaImageContainer.get< Channel >().end())
    {
    if(it->Time==iTime)
      {
        cout  << "The size of the image is: "
              << "\npointer: "<< sizeof( colorImage(it->Image, it->LUT) )
              << "\ndata: "<< sizeof( *colorImage(it->Image, it->LUT) )
              << endl;
      return colorImage(it->Image, it->LUT);
      }
    ++it;
    }
  return NULL;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkSmartPointer<vtkImageData>
GoMegaImageProcessor::
getImageBW(const unsigned int& iTime, const unsigned int& iChannel)
{
  GoMegaImageStructureMultiIndexContainer::index<Channel>::type::iterator it =
      m_MegaImageContainer.get< Channel >().find(iChannel);

  while(it!=m_MegaImageContainer.get< Channel >().end())
    {
    if(it->Time==iTime)
      {
      return it->Image;
      }
    ++it;
    }
  return NULL;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkSmartPointer<vtkImageData>
GoMegaImageProcessor::
getTimeAllChannels(const unsigned int& iTime)
{
  vtkSmartPointer<vtkImageBlend> blendedImage =
      vtkSmartPointer<vtkImageBlend>::New();

  GoMegaImageStructureMultiIndexContainer::index<Time>::type::iterator it =
      m_MegaImageContainer.get< Time >().find(iTime);

  /*
    \todo Nicolas - do sth else....
   */
  double size(0);
  while(it!=m_MegaImageContainer.get< Time >().end())
    {
    ++size;
    ++it;
    }
  it = m_MegaImageContainer.get< Time >().find(iTime);

  vtkIdType i(0);
  while(it!=m_MegaImageContainer.get< Time >().end())
    {
    blendedImage->AddInput(colorImage(it->Image, it->LUT));
    blendedImage->SetOpacity(i, 1/size);
    ++i;
    ++it;
    }
  blendedImage->Update();

  return blendedImage->GetOutput();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkSmartPointer<vtkImageData>
GoMegaImageProcessor::
getChannelAllTimes(const unsigned int& iChannel)
{
  vtkSmartPointer<vtkImageBlend> blendedImage =
      vtkSmartPointer<vtkImageBlend>::New();

  GoMegaImageStructureMultiIndexContainer::index<Channel>::type::iterator it =
      m_MegaImageContainer.get< Channel >().find(iChannel);

  /*
    \todo Nicolas - do sth else....
   */
  double size(0);
  while(it!=m_MegaImageContainer.get< Channel >().end())
    {
    ++size;
    ++it;
    }
  it = m_MegaImageContainer.get< Channel >().find(iChannel);

  vtkIdType i(0);
  while(it!=m_MegaImageContainer.get< Channel >().end())
    {
    blendedImage->AddInput(colorImage(it->Image, it->LUT));
    blendedImage->SetOpacity(i, 1/size);
    ++i;
    ++it;
    }
  blendedImage->Update();

  return blendedImage->GetOutput();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
GoMegaImageProcessor::
setTimePoint(const unsigned int& iTime)
{
  //check if time point exists
  if(iTime >= m_BoundsTime[0] && iTime <= m_BoundsTime[1])
    {
    m_MegaImageReader->SetTimePoint(iTime);
    m_MegaImageReader->Update();
    m_MegaImageContainer.clear();
    }
  else
    {
    return;
    }

  // update the container
  // Get Number of channels from reader
  int numberOfChannels = getNumberOfChannels();

  while(numberOfChannels>0)
    {
    --numberOfChannels;

    // Get useful information from the reader
    // Get Image
    vtkSmartPointer<vtkImageData> image =
        m_MegaImageReader->GetOutput(numberOfChannels);

    // Get Color
    std::vector<std::vector<int> >channelColor =
        m_MegaImageReader->GetChannelColor();


    double random1 = channelColor[numberOfChannels][0];
    double value1 = random1/255;

    double random2 = channelColor[numberOfChannels][1];
    double value2 = random2/255;

    double random3 = channelColor[numberOfChannels][2];
    double value3 = random3/255;

    std::vector<double> color;
    color.push_back(value1);
    color.push_back(value2);
    color.push_back(value3);
    color.push_back(1);

    // Create LUT
    vtkSmartPointer<vtkLookupTable> lut = createLUT(color[0],
                                                    color[1],
                                                    color[2],
                                                    color[3]);
    // set up range
    lut->SetRange(image->GetScalarRange());
    lut->Build();

    // Update the MegaImageStructure
    // image, LUT, channel, time point
    m_MegaImageContainer.insert(GoMegaImageStructure(iTime,
                                                   numberOfChannels,
                                                   lut,
                                                   image,
                                                   color));
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
GoMegaImageProcessor::
setDoppler(const unsigned int& iChannel, const unsigned int& iTime,
           const unsigned int& iPrevious)
{
  //check if time point exists
  if(iTime >= m_BoundsTime[0] && iTime <= m_BoundsTime[1])
    {
    m_MegaImageReader->SetTimePoint(iTime);
    m_MegaImageReader->Update();
    m_MegaImageContainer.clear();
    }
  else
    {
    return;
    }

}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
unsigned int*
GoMegaImageProcessor::
getBoundsTime()
{
  return m_BoundsTime;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
unsigned int*
GoMegaImageProcessor::
getBoundsChannel()
{
  return m_BoundsChannel;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int*
GoMegaImageProcessor::
getExtent()
{
  return m_Extent;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
unsigned int
GoMegaImageProcessor::
getNumberOfTimePoints()
{
  return m_BoundsTime[1] - m_BoundsTime[0] + 1;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
unsigned int
GoMegaImageProcessor::
getNumberOfChannels()
{
  return m_BoundsChannel[1] - m_BoundsChannel[0] + 1;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
unsigned int
GoMegaImageProcessor::
getTimeInterval() const
{
  return m_MegaImageReader->GetTimeInterval();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
unsigned int
GoMegaImageProcessor::
getDopplerStep()
{
  return m_DopplerStep;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
GoMegaImageProcessor::
setDopplerStep(unsigned int iStep)
{
  m_DopplerStep = iStep;
}
//--------------------------------------------------------------------------
