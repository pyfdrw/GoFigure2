/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009-10

 Copyright (c) 2009-10, President and Fellows of Harvard College.
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

#ifndef __ContourMeshContainer_h
#define __ContourMeshContainer_h

#include <QObject>

#include "ContourMeshStructure.h"
#include "StructureHelper.h"

#include "TraceContainerBase.h"

#include "boost/multi_index_container.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/ordered_index.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "boost/lexical_cast.hpp"

#include "vtkProperty.h"
#include "vtkPolyData.h"
#include "vtkActor.h"
#include "vtkMapper.h"
#include "QGoImageView3D.h"

class vtkLookupTable;

namespace boost
{
  typedef multi_index::multi_index_container<
      ContourMeshStructure,
      boost::multi_index::indexed_by<
        boost::multi_index::ordered_non_unique<
          boost::multi_index::tag< TCoord >,
          BOOST_MULTI_INDEX_MEMBER(ContourMeshStructure, unsigned int, TCoord)
          >,
        boost::multi_index::hashed_non_unique<
          boost::multi_index::tag< ActorXY >,
          BOOST_MULTI_INDEX_MEMBER(TraceStructure, vtkActor *, ActorXY)
          >,
        boost::multi_index::hashed_non_unique<
          boost::multi_index::tag< ActorXZ >,
          BOOST_MULTI_INDEX_MEMBER(TraceStructure, vtkActor *, ActorXZ)
          >,
        boost::multi_index::hashed_non_unique<
          boost::multi_index::tag< ActorYZ >,
          BOOST_MULTI_INDEX_MEMBER(TraceStructure, vtkActor *, ActorYZ)
          >,
        boost::multi_index::hashed_non_unique<
          boost::multi_index::tag< ActorXYZ >,
          BOOST_MULTI_INDEX_MEMBER(TraceStructure, vtkActor *, ActorXYZ)
          >,
        boost::multi_index::hashed_non_unique<
          boost::multi_index::tag< Nodes >,
          BOOST_MULTI_INDEX_MEMBER(TraceStructure, vtkPolyData *, Nodes)
          >,
        boost::multi_index::ordered_unique<
          boost::multi_index::tag< TraceID >,
          BOOST_MULTI_INDEX_MEMBER(TraceStructure, unsigned int, TraceID)
          >,
        boost::multi_index::ordered_non_unique<
          boost::multi_index::tag< Highlighted >,
          BOOST_MULTI_INDEX_MEMBER(TraceStructure, bool, Highlighted)
          >,
        boost::multi_index::ordered_non_unique<
          boost::multi_index::tag< Visible >,
          BOOST_MULTI_INDEX_MEMBER(TraceStructure, bool, Visible)
          >
        >
      > MultiIndexContourMeshContainer;
}

/**
  \class ContourMeshContainer
  \brief Wraps a boost multi index container of ContourMeshStructure.
  This class intends to synchronize Contour and Mesh representation in
  the Visualization and in the TableWidget
  \sa ContourMeshStructure QGoTableWidget QGoImageView3D
  */
class ContourMeshContainer:
    public TraceContainerBase< typename boost::MultiIndexContourMeshContainer >
{
  Q_OBJECT
public:

  typedef TraceContainerBase< typename boost::MultiIndexContourMeshContainer > Superclass;
  typedef Superclass::MultiIndexContainerType MultiIndexContainerType;

  typedef MultiIndexContainerType::index< TCoord >::type::iterator
    MultiIndexContainerTCoordIterator;

  //------------------------------------------------------------------------

  /** \brief Constructor. */
  explicit ContourMeshContainer(QObject *iParent,
                                QGoImageView3D *iView);

  /** \brief Destructor. */
  ~ContourMeshContainer();

  /** */
  void SetTimePoint(const unsigned int & iT);

  /**
    \brief Update Visualization of the given TraceIDs
    \tparam TContainer Container of TraceIDs
    \param[in] iList input container of TraceIDs
    \param[in] iContour
  */
  template< class TContainer >
  void UpdateVisualizationForGivenIDs(TContainer iList,
                                      const bool & iContour)
  {
    typename TContainer::iterator it = iList.begin();

    while ( it != iList.end() )
      {
      MultiIndexContainerTraceIDIterator id_it =
        m_Container.get< TraceID >().find( static_cast< unsigned int >( *it ) );

      if ( id_it != m_Container.get< TraceID >().end() )
        {
        ContourMeshStructure temp(*id_it);
        temp.Highlighted = false;
        temp.Visible = false;

        vtkProperty *tproperty = vtkProperty::New();
        tproperty->SetColor(id_it->rgba[0], id_it->rgba[1], id_it->rgba[2]);
        tproperty->SetOpacity(id_it->rgba[3]);

        vtkPolyData *nodes = id_it->Nodes;
        if ( nodes )
          {
          temp.Visible =
            ( static_cast< unsigned int >( m_TCoord ) == id_it->TCoord );

          std::vector< vtkActor * > actor;

          if ( iContour )
            {
            int dir = ComputeDirectionFromContour(nodes);

            if ( dir != -1 )
              {
              m_ImageView->EnableContourWidget(true);
              m_ImageView->InitializeContourWidgetNodes(dir, nodes);

              vtkPolyData *trace = vtkPolyData::New();
              trace->ShallowCopy(
                m_ImageView->GetContourRepresentationAsPolydata(dir) );

              actor = this->m_ImageView->AddContour(trace, tproperty);

              m_ImageView->ReinitializeContourWidget();
              m_ImageView->EnableContourWidget(false);
              }
            }
          else
            {
            actor = this->m_ImageView->AddContour(nodes, tproperty);
            }

          temp.ActorXY = actor[0];
          temp.ActorXZ = actor[1];
          temp.ActorYZ = actor[2];
          temp.ActorXYZ = actor[3];

          typedef void ( QGoImageView3D::*ImageViewMember )(const int &, vtkActor *);
          ImageViewMember f;

          if ( temp.Visible )
            {
            f = &QGoImageView3D::AddActor;
            }
          else
            {
            f = &QGoImageView3D::RemoveActor;
            }

          for ( int i = 0; i < 4; i++ )
            {
            ( m_ImageView->*f )(i, actor[i]);
            }
          }
        else
          {
          temp.Visible = false;
          }

        m_Container.get< TraceID >().replace(id_it, temp);
        }
      ++it;
      }
  }

  /** \brief Display all elements for a given time point
  *   \param[in] iT time point
  */
  void ShowActorsWithGivenTimePoint(const unsigned int & iT);

  /**
 *  \brief Change elements (in between iBegin and iEnd ) visibility
 *  \param[in] iBegin first element
 *  \param[in] iEnd last element
 *  \param[in] iVisibility
 */
  void
  ChangeActorsVisibility(
    MultiIndexContainerTCoordIterator iBegin,
    MultiIndexContainerTCoordIterator iEnd,
    const bool & iVisibility);

  /** \brief Update Current Element by providing all required informations
  from the visualization.
  \param[in] iActors
  \param[in] iNodes
  \param[in] iT
  \param[in] iHighlighted
  \param[in] iVisible
  \see ContourMeshStructure
  */
  void UpdateCurrentElementFromVisu(std::vector< vtkActor * > iActors,
                                    vtkPolyData *iNodes,
                                    const unsigned int & iT,
                                    const bool & iHighlighted,
                                    const bool & iVisible);

  /**
  \brief Remove all actors (elements) from the scene for a given time point
  \param[in] iT
  */
  void RemoveActorsWithGivenTimePoint(const unsigned int & iT);

  /**
    \brief Add all actors (elements) from the scene for a given time point
  */
  void AddActorsWithGivenTimePoint(const unsigned int & iT);

  /**
  \brief Returns the direction of a given contour vtkPolyData.
  This static method is supposed to be used when dealing with contours.
  \param[in] iContour
  \return 0 if z coordinates are constant
  \return 1 if y coordinates are constant
  \return 2 if x coordinates are constant
  \return -1 else
  */
  static
  int
  ComputeDirectionFromContour(vtkPolyData *iContour)
  {
    double bounds[6];

    iContour->GetBounds(bounds);

    return ComputeDirectionFromBounds< double >(bounds);
  }

  /**
  \brief Returns the direction of a given element given its bounding box.
  This static method is supposed to be used when dealing with contours.
  \return 0 if z coordinates are constant
  \return 1 if y coordinates are constant
  \return 2 if x coordinates are constant
  \return -1 else
  */
  template< typename T >
  static
  int
  ComputeDirectionFromBounds(T *iBounds)
  {
    int oDir = -1;

    for ( int i = 0; i < 3; i++ )
      {
      if ( iBounds[2 * i] == iBounds[2 * i + 1] )
        {
        oDir = 2 - i;
        }
      }

    return oDir;
  }

  template< typename T >
  static
  int
  ComputeDirectionFromBounds(const std::vector< T > & iBounds)
  {
    int oDir = -1;

    if ( iBounds.size() == 6 )
      {
      for ( int i = 0; i < 3; ++i )
        {
        if ( iBounds[2 * i] == iBounds[2 * i + 1] )
          {
          oDir = 2 - i;
          }
        }
      }

    return oDir;
  }

  /**
  \brief Update highlighting property of one element given one actor.
  \param[in] iActor Actor of the element to be modified
  \return true if the element exists
  \return false else
  */
  template< class TActor >
  bool UpdateElementHighlightingWithGivenActor(vtkActor *iActor)
  {
    unsigned TraceId;
    Qt::CheckState state;
    bool oValue =
        Superclass::UpdateElementHighlightingWithGivenActor< TActor >( iActor,
                                                                 TraceId,
                                                                 state );
    if( oValue )
      {
      emit TracePicked(TraceId, state);
      }
    return oValue;
  }

  /** \brief Update element Visibility property given one actor.
  \tparam TActor either ActorXY, ActorXZ, ActorYZ, ActorXYZ depending on the view
  \param[in] iActor provided actor
  \return true if iActor is in the container
  \return false else
  */
  template< class TActor >
  bool UpdateElementVisibilityWithGivenActor(vtkActor *iActor)
  {
    unsigned TraceId;
    Qt::CheckState state;
    bool oValue =
      Superclass::UpdateElementVisibilityWithGivenActor< TActor >( iActor,
                                                                   TraceId,
                                                                   state );
    if( oValue )
      {
      emit TraceVisibilityChanged(TraceId, state);
      }

    return oValue;
  }

  /**
  \brief Update highlighting property of one element given one actor.
  \param[in] iActor Actor of the element to be modified
  \param[in] iState Visibility to applied to the element
  \return true if the element exists
  \return false else
  */
  template< class TActor >
  bool UpdateElementVisibilityWithGivenActor(vtkActor *iActor, bool iState)
  {
    if ( iActor )
      {
      typedef typename MultiIndexContainerType::index< TActor >::type::iterator
      IteratorType;
      IteratorType it = m_Container.get< TActor >().find(iActor);

      if ( it != m_Container.get< TActor >().end() )
        {
        if ( it->Visible != iState )
          {
          it->SetActorVisibility( iState );

          ContourMeshStructure tempStructure(*it);
          tempStructure.Visible = iState;

          Qt::CheckState State;

          // Note: it->Highlighted is the status before picking the actor
          if ( iState )
            {
            State = Qt::Checked;
            }
          else
            {
            State = Qt::Unchecked;
            }

          m_Container.get< TActor >().replace(it, tempStructure);
          //m_ImageView->UpdateRenderWindows();

          emit TraceVisibilityChanged(it->TraceID, State);
          }

        return true;
        }
      }

    return false;
  }

  //-------------------------------------------------------------------------

  /**
    \brief Remove the element which TraceId = iId
    <ul>
      <li>from the visualization
      <li>from the container
      <li>from memory (release memory)
    </ul>
    \param[in] iId TraceID of the element to be deleted
    \return true if the element was present in the container.
  */
  bool DeleteElement(const unsigned int & iId);

  /**
  \overload DeleteElement(const unsigned int & iId)
  */
  bool DeleteElement(MultiIndexContainerTraceIDIterator iIter);

  /** \brief Delete all highlighted elements
  \return the list of TraceIDs of such elements
  */
  std::list< unsigned int > DeleteAllHighlightedElements();

   /**
    \return the traceIDs with TCoord = iTimePoint
    \param[in] iTimePoint timepoint for which the traceIDs are needed
    */
  std::list< unsigned int > GetElementsTraceIDForGivenTimePoint(unsigned int iTimePoint);


  /*
   * \brief Returns the meshes positions
   * \param[in] iMeshID IDs of the meshes of interest
   * \return map containing the coordinates of the meshes of interest
   */
  std::map< unsigned int, double* > GetMeshesPoints( std::list< unsigned int> iMeshID );

public slots:

  /** \brief Change elements highlighting property given a list of TraceIDs
  and the new status.
    \param[in] iList list of TraceIDs
    \param[in] iCheck */
  void UpdateElementHighlightingWithGivenTraceIDs( const QStringList& iList,
                                                   const Qt::CheckState& iCheck );

  /** \brief Change elements visibility property given a list of TraceIDs
  and the new status.
    \param[in] iList list of TraceIDs
    \param[in] iCheck */
  void UpdateElementVisibilityWithGivenTraceIDs( const QStringList& iList,
                                                 const Qt::CheckState& iCheck );

signals:
  /** \brief When one contour / mesh has been picked (highlighted) from the visualization */
  void TracePicked(unsigned int, Qt::CheckState);

  /** \brief When one contour / mesh's visibility has been changed from the visualization */
  void TraceVisibilityChanged(unsigned int, Qt::CheckState);

protected:
  unsigned int m_TCoord;

private:
  Q_DISABLE_COPY(ContourMeshContainer);
};

#endif // CONTOURMESHCONTAINER_H
