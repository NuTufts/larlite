#ifndef GETAVERAGES3D_CXX
#define GETAVERAGES3D_CXX

#include "GetAverages3D.h"
#include "TPrincipal.h"
#include "ClusterRecoUtil/Base/CRUException.h"
#include <map>

namespace cluster3D {

void GetAverages3D::do_params_fill(cluster3D_params & cluster) {

  // Use principal component analysis for a lot of this
  // So, use root's built in class
  TPrincipal fPrincipal(3, "D");

  // Set the total number of hits
  cluster.N_points = cluster.point_vector.size();

  // Find out the mean,  etc.
  for (auto& point : cluster.point_vector) {
    // std::cout << "This point has charge " <<  point.charge << "\n";

    // Cast the wire/time information into a double array for prin. comp. analysis
    double data[3];
    data[0] = point.X();
    data[1] = point.Y();
    data[2] = point.Z();
    // Add this data point to the PCA
    fPrincipal.AddRow(data);


  }

  // Check that there are mean values before setting them
  if (fPrincipal.GetMeanValues()->GetNrows() < 3) {
    throw cluster::CRUException();
    return;
  }

  // Set the mean values from the PCA, which calculates them anyways
  cluster.mean_x = (* fPrincipal.GetMeanValues())[0];
  cluster.mean_y = (* fPrincipal.GetMeanValues())[1];
  cluster.mean_z = (* fPrincipal.GetMeanValues())[2];

  // Run the PCA analysis
  fPrincipal.MakePrincipals();

  // Save the output eigen vectors
  cluster.eigenvalue_principal = (* fPrincipal.GetEigenValues() )[0];
  cluster.eigenvalue_secondary = (* fPrincipal.GetEigenValues() )[1];
  cluster.eigenvalue_tertiary  = (* fPrincipal.GetEigenValues() )[2];

  cluster.principal_dir[0] = (* fPrincipal.GetEigenVectors())[0][0];
  cluster.principal_dir[1] = (* fPrincipal.GetEigenVectors())[1][0];
  cluster.principal_dir[2] = (* fPrincipal.GetEigenVectors())[2][0];

  cluster.secondary_dir[0] = (* fPrincipal.GetEigenVectors())[0][1];
  cluster.secondary_dir[1] = (* fPrincipal.GetEigenVectors())[1][1];
  cluster.secondary_dir[2] = (* fPrincipal.GetEigenVectors())[2][1];

  cluster.tertiary_dir[0] = (* fPrincipal.GetEigenVectors())[0][2];
  cluster.tertiary_dir[1] = (* fPrincipal.GetEigenVectors())[1][2];
  cluster.tertiary_dir[2] = (* fPrincipal.GetEigenVectors())[2][2];


  double rmsx = 0.0;
  double rmsy = 0.0;
  double rmsz = 0.0;

  for (auto& point : cluster.point_vector) {
    // First, abuse this loop to calculate rms in x and y
    rmsx += pow(cluster.mean_x - point.X(), 2) / cluster.N_points;
    rmsy += pow(cluster.mean_y - point.Y(), 2) / cluster.N_points;
    rmsz += pow(cluster.mean_z - point.Z(), 2) / cluster.N_points;

  }

  cluster.rms_x = sqrt(rmsx);
  cluster.rms_y = sqrt(rmsy);
  cluster.rms_z = sqrt(rmsz);

  return;

}

} // cluster

#endif