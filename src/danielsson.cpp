#include "danielsson.h"
// ==================================================================================================

// Distance de m a cell (indices vers pts)
// alpha-> coordonnees barycentriques du point le plus proche
// sum(alpha_i)=1
// inside: le point le plus proche est dedans (alpha_i!=0 pour tout i)

// Fn secondaire: il ne reste que nb points (pour les autres alpha=0)

using namespace std;

static double dpc(const dpoint&m ,const dpoint *pts,const ipoint& cell,dpoint& alphas,int nb,ipoint& idx,bool& inside)
{
    if(nb == 1)
    {
        alphas[idx[0]]=1;
        return (m-pts[cell[idx[0]]]).norme();
    }
    // Resoud H=sum(alpha_i A_i), sum(alpha_i)=1, et HM.(A_i-A_0)=0
    dpoint A0Ai[DIM]; // A_i-A_0
    for( int i=1; i<nb; i++ )
    {
        A0Ai[i]=pts[cell[idx[i]]]-pts[cell[idx[0]]];
    }
    dpoint A0M=m-pts[cell[idx[0]]]; // M-A_0
    if(nb == 2)
    {
        alphas[idx[1]]=(A0M*A0Ai[1])/(A0Ai[1]*A0Ai[1]);
        alphas[idx[0]]=1-alphas[idx[1]];
    } 
    else if (nb==3) 
    {
        // Systeme ci dessous resolution directe (systeme 2x2)
        double a00=A0Ai[1]*A0Ai[1];
        double a10=A0Ai[1]*A0Ai[2];
        double a11=A0Ai[2]*A0Ai[2];
        double b0=A0M*A0Ai[1];
        double b1=A0M*A0Ai[2];
        double d=a00*a11-a10*a10;
        assert(d!=0);
        alphas[idx[1]]=(b0*a11-b1*a10)/d;
        alphas[idx[2]]=(a00*b1-a10*b0)/d;
        alphas[idx[0]]=1-alphas[idx[1]]-alphas[idx[2]];
    }
    else
    {
        // 3 inconnues ou plus -> systeme
        // Resoud Ax=b avec: A(i,j)=A0Ai.AjA0, x=(alpha_1,alpha_2, ...), b=A0M.A0Ai
        cerr << "dim>=4 dans danielsson : a programmer" << endl;
        exit(0);
    }
    // Si alpha_i<0 -> le ramene a 0 et recursion
    // NB: traite le cas >0 car si alpha_i>1 alors alpha_j<0 pour un j
    for (int i=0;i<nb;i++)
    {
        if (alphas[idx[i]]<0)
        {
            inside=false;
            alphas[idx[i]]=0;
            swap(idx[i],idx[nb-1]);
            return dpc(m,pts,cell,alphas,nb-1,idx,inside);
        }
    }
    // Sinon: distance HM
    dpoint MH=-A0M;
    for (int i=1;i<nb;i++)
    {
        MH=MH+alphas[idx[i]]*A0Ai[i];
    }
    return MH.norme();
}

// Fn principale
double dist_point_cell(const dpoint&m ,const dpoint *pts,const ipoint& cell,dpoint& alphas,bool& inside)
{
    ipoint idx;
    for (dim_t i=0;i<DIM;i++)
    {
        idx[i]=i;
    }
    inside=true;
    return dpc(m,pts,cell,alphas,DIM,idx,inside);
}

//============================================================================
// Utile pour:
// - mesh to distance
// - pointeur vers mesh le plus proche: un peu lent mais une fois pour toute -> utile si la surface
//   ne bouge plus...
// - Attention: cell[i] = -1 possible si narrowband sans contour dedans (arrive quand disparition, reinit, etc.)
// ======================================================================

static inline int sgn(double s) { return (s>0)?1:(s<0)?-1:0; }

#include <list>
using namespace std;

double dist_point_mesh(const dpoint&m ,const Mesh &mesh,dpoint& alphas,int &nearestNumber)
{
    double distmin=DBL_MAX;
    ipoint t;
    bool inside;
    double distance;
    dpoint alphasLoop;

    for(int i=0; i<mesh.nbTrgs(); i++)
    {
        t=mesh.getTrg(i);
        distance=dist_point_cell(m ,& mesh.getPt(0),t,alphasLoop,inside);
        if(distance < distmin)
        {
            distmin=distance;
            nearestNumber=i;
            alphas=alphasLoop;
        }

    }

    return distmin;
}

