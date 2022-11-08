/*
Copyright (C) 2018 - 2021 by the authors of the World Builder code.

This file is part of the World Builder.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "world_builder/objects/cubic_spline.h"

#include "world_builder/nan.h"

using namespace WorldBuilder;

namespace WorldBuilder
{
  namespace Objects
  {
    CubicSpline::CubicSpline(std::vector<double> &y)
    {
      int n = y.size()-1;
      std::vector<double> a;
      a.insert(a.begin(), y.begin(), y.end());
      std::vector<double> b(n,NaN::DSNAN);
      std::vector<double> d(n,NaN::DSNAN);

      std::vector<double> alpha;
      alpha.push_back(0);
      for (int i = 1; i < n; ++i)
        alpha.push_back( 3*(a[i+1]-a[i]) - 3*(a[i]-a[i-1])  );

      std::vector<double> c(n+1,NaN::DSNAN);
      std::vector<double> l(n+1,NaN::DSNAN);
      std::vector<double> mu(n+1,NaN::DSNAN);
      std::vector<double> z(n+1,NaN::DSNAN);
      l[0] = 1;
      mu[0] = 0;
      z[0] = 0;

      for (int i = 1; i < n; ++i)
        {
          l[i] = 4.-mu[i-1];
          mu[i] = 1./l[i];
          z[i] = (alpha[i]-z[i-1])/l[i];
          //std::cout << i << ": l = " << l[i] << ", mu = " << mu[i] << ", z = " << z[i] << std::endl;
        }

      l[n] = 1;
      z[n] = 0;
      c[n] = 0;

      for (int j = n-1; j >= 0; --j)
        {
          c[j] = z [j] - mu[j] * c[j+1];
          b[j] = (a[j+1]-a[j])-(c[j+1]+2*c[j])/3.;
          d[j] = (c[j+1]-c[j])/3.;
        }

      m.resize(n, {NaN::DSNAN,NaN::DSNAN,NaN::DSNAN,NaN::DSNAN});
      //std::cout << "n = " << n << std::endl;
      for (int i = 0; i < n; ++i)
        {
          m[i][3] = a[i];
          m[i][2] = b[i];
          m[i][1] = c[i];
          m[i][0] = d[i];
        }
    }
  }
}