// Darcy bilinear and linear forms.
//
// REMEMBER: the matrix corresponding to a(u,v) is Aij = a(\phi_j, \phi_i)
#ifndef _DARCY_OPERATORSX_
#define _DARCY_OPERATORSX_ 1

#include "Core.h"
#include "LevelSetHandler.h"
#include "MeshHandler.h"
#include "FractureHandler.h"
#include "BCHandler.h"

namespace getfem
{

// Defining unit normal on a level set ------------------------------------
class level_set_unit_normal : public getfem::nonlinear_elem_term
{
    const getfem::mesh_fem& mf;
    scalarVector_Type U;
    size_type N;
    base_matrix gradU;
    bgeot::base_vector coeff;
    bgeot::multi_index sizes_;

public:

    level_set_unit_normal ( const getfem::mesh_fem& mf_,
                            const scalarVector_Type& U_ );

    inline const bgeot::multi_index& sizes ( ) const
    {
        return sizes_;
    }

    virtual void compute ( getfem::fem_interpolation_context& ctx,
                           bgeot::base_tensor& t );
};

// costruice il vettore velocità in un punto dati i gdl di velocità usanto le funzioni
// di base nel vettore di punti desiderato

void interpolateVelocity ( scalarVector_Type& V,
                           const base_node& point,
                           const size_type &elem,
                           const scalarVector_Type& gdl,
                           const MeshHandlerPtr_Type& mesh,
                           const sparseMatrixPtr_Type& globalMatrix );

//calcola la lunghezza dei lati della frattura
void stimaLati ( scalarVector_Type& latiF,
                 const FractureHandlerPtr_Type& fracture );

/* mappa fra p frattura e p volume
 MM è una matrice per passare dagli elementi della M_mediumMesh frattura agli elementi della M_mediumMesh volume */
void interpolationMatrix ( sparseMatrixPtr_Type& MM,
                           const MeshHandlerPtr_Type& mesh,
                           const FractureHandlerPtr_Type& fracture,
                           const size_type& regionID );

void interpolationMatrix ( sparseMatrixPtrContainer_Type& interpolationMatrices,
                           const MeshHandlerPtr_Type& mesh,
                           FracturePtrContainer_Type& fractures,
                           const IntersectData_Type& intersect );

// per il precondizionatore e per la concentrazione, matrice di massa sulle p
void massL2 ( sparseMatrixPtr_Type& M,
              const MeshHandlerPtr_Type& mesh,
              const scalarVector_Type& inv_eta,
              const size_type& uncutRegionFlag );

void massL2 ( sparseMatrixPtr_Type& M,
              const MeshHandlerPtr_Type& mesh,
              const FractureHandlerPtr_Type& fracture,
              const scalarVector_Type& inv_eta,
              const size_type& cutRegionFlag );

// per il precondizionatore - e per la concentrazione, matrice di massa sulle p, per la frattura
void massL2 ( sparseMatrixPtr_Type& M,
              const FractureHandlerPtr_Type& fracture,
              const scalarVector_Type& dens );

// Operator A11:
void massHdiv ( sparseMatrixPtr_Type& M,
                const MeshHandlerPtr_Type& mesh,
                const scalarVectorContainer_Type& mediumEtaInterpolated,
                const size_type& uncutRegionFlag );

void massHdiv ( sparseMatrixPtr_Type& M,
                const MeshHandlerPtr_Type& mesh,
                const FractureHandlerPtr_Type& fracture,
                const scalarVector_Type& fractureEtaNormalOnMedium,
                const scalarVectorContainer_Type& mediumEtaInterpolated,
                const size_type& cutRegionFlag );

void massHdiv ( sparseMatrixPtr_Type& M,
                const IntersectData_Type& intersect,
                const MeshHandlerPtr_Type& mesh,
                const scalarVectorContainer_Type& mediumEtaInterpolated,
                const std::vector < scalarVector_Type* >& fracturesEta );

void darcy_E ( sparseMatrixPtr_Type& E,
               const MeshHandlerPtr_Type& mesh,
               const FractureHandlerPtr_Type& fracture,
               const size_type& cutRegionFlag );

void darcy_E ( sparseMatrixPtr_Type& E,
               const FractureHandlerPtr_Type& fracture,
               const IntersectData_Type& intersect,
               const MeshHandlerPtr_Type& mesh,
               const scalar_type& localFractureID );

void nitsche ( sparseMatrixPtr_Type& M,
               const MeshHandlerPtr_Type& mesh,
               const scalarVectorContainer_Type& mediumInvKInterpolated,
               const scalar_type& penaltyParameterVelocity,
               const sizeVector_Type &extBoundaryUncut,
               const size_type& uncutRegionFlag );

void nitsche ( sparseMatrixPtr_Type& M,
               const MeshHandlerPtr_Type& mesh,
               const FractureHandlerPtr_Type& fracture,
               const scalarVectorContainer_Type& mediumInvKInterpolated,
               const scalar_type& penaltyParameterVelocity,
               const sizeVector_Type &ExtBoundary_cut );

//questo tiene conto del termine sorgenre
void sourceL2 ( scalarVectorPtr_Type& D,
                const scalarVectorPtr_Type& source,
                const MeshHandlerPtr_Type& mesh,
                const size_type& uncutRegionFlag );

//questo tiene conto del termine sorgenre
void sourceL2 ( scalarVectorPtr_Type& D,
                const scalarVectorPtr_Type& source,
                const MeshHandlerPtr_Type& mesh,
                const FractureHandlerPtr_Type& fracture,
                const size_type& cutRegionFlag );

void sourceL2 ( scalarVectorPtr_Type& V,
                const scalarVectorPtr_Type& source,
                const IntersectData_Type& intersect,
                const MeshHandlerPtr_Type& mesh );

//matrice tau tau per la frattura
void darcy_A11F ( sparseMatrixPtr_Type& M,
                  const FractureHandlerPtr_Type& fracture,
                  const scalar_type& gammaU,
                  const scalarVector_Type& invKTangentialInterpolated,
                  const sizeVector_Type &ExtBoundary,
                  const size_type& uncutRegionFlag );

//matrice tau tau per la frattura intersecata
void darcy_A11F ( sparseMatrixPtr_Type& M,
                  const FractureHandlerPtr_Type& fracture,
                  const scalarVector_Type& invKTangentialInterpolated,
                  const FractureHandlerPtr_Type& otherFracture,
                  const size_type& cutRegionFlag );

void divHdiv ( sparseMatrixPtr_Type& M,
               const MeshHandlerPtr_Type& mesh,
               const size_type& uncutRegionFlag );

void divHdiv ( sparseMatrixPtr_Type& M,
               const MeshHandlerPtr_Type& mesh,
               const FractureHandlerPtr_Type& fracture,
               const size_type& uncutRegionFlag );

void divHdiv ( sparseMatrixPtr_Type& M,
               const IntersectData_Type& intersect,
               const MeshHandlerPtr_Type& mesh );

//lo stesso per la frattura
void darcy_A12F ( sparseMatrixPtr_Type& M,
                  const FractureHandlerPtr_Type& fracture,
                  const size_type& uncutRegionFlag );

//lo stesso per la frattura
void darcy_A12F ( sparseMatrixPtr_Type& M,
                  const FractureHandlerPtr_Type& fracture,
                  const FractureHandlerPtr_Type& otherFracture,
                  const size_type& cutRegionFlag );

//termine di trasporto
void advectHdiv ( sparseMatrixPtr_Type& M,
                  const MeshHandlerPtr_Type& mesh,
                  const scalarVectorContainer_Type& mediumMu,
                  const scalarVectorPtr_Type& velocity,
                  const size_type& uncutRegionFlag );

//termine di trasporto
void advectHdiv ( sparseMatrixPtr_Type& M,
                  const MeshHandlerPtr_Type& mesh,
                  const FractureHandlerPtr_Type& fracture,
                  const scalarVectorContainer_Type& mediumMu,
                  const scalarVectorPtr_Type& velocityIn,
                  const scalarVectorPtr_Type& velocityOut,
                  const size_type& cutRegionFlag );

// termine di trasporto nella frattura
void darcy_DF ( sparseMatrixPtr_Type& M,
                const getfem::mesh_im& im,
                const getfem::mesh_fem& mfv,
                const getfem::mesh_fem& mfp,
                const scalarVector_Type& invD,
                const scalarVector_Type& Fxx,
                const scalarVectorPtr_Type& vel,
                const mesh_region& uncutRegion );

//questa fa la parte di termine noto che contiene le condizioni al contorno
//il termine di pressione entra come condizione naturale, quello di velocità con una penalizzazione di Nitzche
void stressRHS ( scalarVectorPtr_Type &Bstress,
                 const MeshHandlerPtr_Type& mesh,
                 const BCHandlerPtr_Type& bcHandler,
                 const scalarVectorPtr_Type &Pneumann );

//questa fa la parte di termine noto che contiene le condizioni al contorno
//il termine di pressione entra come condizione naturale, quello di velocità con una penalizzazione di Nitzche
void nitscheRHS ( scalarVectorPtr_Type &Bvel,
                  const MeshHandlerPtr_Type& mesh,
                  const BCHandlerPtr_Type& bcHandler,
                  const scalar_type& eta,
                  const scalar_type& gammaU,
                  const scalarVectorPtr_Type &v_diri );

void stressRHS ( scalarVectorPtr_Type &Bstress,
                 const MeshHandlerPtr_Type& mesh,
                 const BCHandlerPtr_Type& bcHandler,
                 const FractureHandlerPtr_Type& fracture,
                 const size_type& fractureShift,
                 const scalarVectorPtr_Type &PneumannIn,
                 const scalarVectorPtr_Type &PneumannOut );

void nitscheRHS ( scalarVectorPtr_Type &Bvel,
                  const MeshHandlerPtr_Type& mesh,
                  const BCHandlerPtr_Type& bcHandler,
                  const FractureHandlerPtr_Type& fracture,
                  const size_type& fractureShift,
                  const scalar_type& invK,
                  const scalar_type& gammaU,
                  const scalarVectorPtr_Type &v_diri );

//stesso lavoro con la frattura - più semplice
void darcy_dataF ( scalarVectorPtr_Type &Bstress,
                   scalarVectorPtr_Type &Bvel,
                   const BCHandlerPtr_Type& bcHandler,
                   const FractureHandlerPtr_Type& fracture,
                   const scalar_type& gammaU,
                   const scalar_type& invK,
                   const scalarVectorPtr_Type& Pneumann,
                   const scalarVectorPtr_Type& v_diri );

//termine sorgente per la frattura
void assembling_Source_BoundaryF ( scalarVectorPtr_Type& D,
                                   const scalarVectorPtr_Type& source,
                                   const FractureHandlerPtr_Type& fracture,
                                   const size_type& uncutRegionFlag );

//termine sorgente per la frattura
void assembling_SourceF ( scalarVectorPtr_Type& D,
                          const scalarVectorPtr_Type& source,
                          const FractureHandlerPtr_Type& fracture,
                          const FractureHandlerPtr_Type& otherFracture,
                          const size_type& cutRegionFlag );

void coupleFractures ( sparseMatrixPtr_Type& M,
                       const FracturesSetPtr_Type& fractures );

void velocityJump ( sparseMatrixPtr_Type& M,
                    const FractureHandlerPtr_Type& fracture,
                    const FractureHandlerPtr_Type& otherFracture,
                    const size_type& convex );

// L2 norm che tiene conto della frattura
scalar_type extended_L2_norm ( const scalarVectorPtr_Type& V1,
                               const LevelSetHandlerPtr_Type& levelSet,
                               const MeshHandlerPtr_Type& mesh,
                               const sizeVector_Type &eXt_dof,
                               const size_type& cutRegionFlag,
                               const size_type& uncutRegionFlag );

// L2 norma della divergenza
scalar_type extended_divL2_norm ( const scalarVectorPtr_Type& V1,
                                  const LevelSetHandlerPtr_Type& levelSet,
                                  const MeshHandlerPtr_Type& mesh,
                                  const sizeVector_Type &eXt_dof,
                                  const size_type& cutRegionFlag,
                                  const size_type& uncutRegionFlag );

// calcola il flusso uscente da parte del bordo del dominio
void flux_out ( scalar_type& flux2d,
                const scalarVector_Type& V1,
                const LevelSetHandlerPtr_Type& levelSet,
                const MeshHandlerPtr_Type& mesh,
                const sizeVector_Type &eXt_dof,
                const sizeVector_Type &Boundary_cut,
                const sizeVector_Type &Boundary_uncut );

// L2 norm con peso
scalar_type extended_L2_norm ( const scalarVector_Type& V1,
                               const scalarVector_Type& peso,
                               const mesh_im& mim,
                               const mesh_im& mimIn,
                               const mesh_im& mimOut,
                               const mesh_fem& mf,
                               const mesh_fem& mf2,
                               const scalarVector_Type& ls_el,
                               const sizeVector_Type& eXt_dof,
                               const mesh_region& cutRegion,
                               const mesh_region& uncutRegion );

} // namespace getfem

#endif
