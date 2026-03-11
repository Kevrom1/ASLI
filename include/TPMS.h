/* =============================================================================
   Copyright (C) 2022 KU Leuven
   ASLI (A Simple Lattice Infiller) - Extended version with additional TPMS types
   Original: https://github.com/tpms-lattice/ASLI
   License: GNU Affero General Public License
   ============================================================================= */

#ifndef TPMS_H
#define TPMS_H

#include <string>
#include <cmath>
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* -----------------------------------------------------------------------------
   TPMS implicit surface functions
   All functions take (x, y, z) coordinates scaled by 2*pi/cellSize
   and return the field value. The isosurface is at f(x,y,z) = isovalue.
   ----------------------------------------------------------------------------- */
class TPMS {
public:

  /* ---------------------------------------------------------------------------
     Original ASLI TPMS types
     ------------------------------------------------------------------------- */

  // Gyroid (Schoen, 1970)
  // f = sin(x)cos(y) + sin(y)cos(z) + sin(z)cos(x)
  static double gyroid(double x, double y, double z) {
    return std::sin(x) * std::cos(y)
         + std::sin(y) * std::cos(z)
         + std::sin(z) * std::cos(x);
  }

  // Schwarz Primitive / P-surface (Schwarz, 1865)
  // f = cos(x) + cos(y) + cos(z)
  static double schwarzP(double x, double y, double z) {
    return std::cos(x) + std::cos(y) + std::cos(z);
  }

  // Schwarz Diamond / D-surface (Schwarz, 1865)
  // f = cos(x)cos(y)cos(z) - sin(x)sin(y)sin(z)
  static double schwarzD(double x, double y, double z) {
    return std::cos(x) * std::cos(y) * std::cos(z)
         - std::sin(x) * std::sin(y) * std::sin(z);
  }

  /* ---------------------------------------------------------------------------
     Extended TPMS types — NEW
     ------------------------------------------------------------------------- */

  // Neovius surface (Neovius, 1883)
  // f = 3(cos(x) + cos(y) + cos(z)) + 4*cos(x)*cos(y)*cos(z)
  // Higher genus surface with complex channel network
  static double neovius(double x, double y, double z) {
    return 3.0 * (std::cos(x) + std::cos(y) + std::cos(z))
         + 4.0 * std::cos(x) * std::cos(y) * std::cos(z);
  }

  // I-WP surface (Schoen I-graph / Wrapped Package, 1970)
  // f = 2*(cos(x)cos(y) + cos(y)cos(z) + cos(z)cos(x)) - (cos(2x) + cos(2y) + cos(2z))
  // High surface-area, excellent for heat exchangers
  static double iwp(double x, double y, double z) {
    return 2.0 * (std::cos(x) * std::cos(y)
                + std::cos(y) * std::cos(z)
                + std::cos(z) * std::cos(x))
         - (std::cos(2.0 * x) + std::cos(2.0 * y) + std::cos(2.0 * z));
  }

  // Lidinoid surface (Lidin & Larsson, 1990)
  // f = 0.5*(sin(2x)cos(y)sin(z) + sin(2y)cos(z)sin(x) + sin(2z)cos(x)sin(y))
  //   - 0.5*(cos(2x)cos(2y) + cos(2y)cos(2z) + cos(2z)cos(2x)) + 0.15
  // Chiral surface, no reflection symmetry
  static double lidinoid(double x, double y, double z) {
    return 0.5  * (std::sin(2.0 * x) * std::cos(y) * std::sin(z)
                 + std::sin(2.0 * y) * std::cos(z) * std::sin(x)
                 + std::sin(2.0 * z) * std::cos(x) * std::sin(y))
         - 0.5  * (std::cos(2.0 * x) * std::cos(2.0 * y)
                 + std::cos(2.0 * y) * std::cos(2.0 * z)
                 + std::cos(2.0 * z) * std::cos(2.0 * x))
         + 0.15;
  }

  // Split-P surface (Schoen, 1970)
  // f = 1.1*(sin(2x)sin(z)cos(y) + sin(2y)sin(x)cos(z) + sin(2z)sin(y)cos(x))
  //   - 0.2*(cos(2x)cos(2y) + cos(2y)cos(2z) + cos(2z)cos(2x))
  //   - 0.4*(cos(2x) + cos(2y) + cos(2z))
  // P-surface variant with split topology
  static double splitP(double x, double y, double z) {
    return 1.1  * (std::sin(2.0 * x) * std::sin(z) * std::cos(y)
                 + std::sin(2.0 * y) * std::sin(x) * std::cos(z)
                 + std::sin(2.0 * z) * std::sin(y) * std::cos(x))
         - 0.2  * (std::cos(2.0 * x) * std::cos(2.0 * y)
                 + std::cos(2.0 * y) * std::cos(2.0 * z)
                 + std::cos(2.0 * z) * std::cos(2.0 * x))
         - 0.4  * (std::cos(2.0 * x) + std::cos(2.0 * y) + std::cos(2.0 * z));
  }

  // Fisher-Koch S surface (Fischer & Koch, 1987)
  // f = cos(2x)sin(y)cos(z) + cos(x)cos(2y)sin(z) + sin(x)cos(y)cos(2z)
  // High strength-to-density ratio, suitable for biomedical scaffolds
  static double fischerKochS(double x, double y, double z) {
    return std::cos(2.0 * x) * std::sin(y) * std::cos(z)
         + std::cos(x) * std::cos(2.0 * y) * std::sin(z)
         + std::sin(x) * std::cos(y) * std::cos(2.0 * z);
  }

  // Schwarz Hexagonal / H-surface (Schwarz, 1865)
  // f = sin(x)sin(y+pi/3) + sin(y)sin(z+pi/3) + sin(z)sin(x+pi/3)
  // Hexagonal symmetry group
  static double schwarzH(double x, double y, double z) {
    const double pi3 = M_PI / 3.0;
    return std::sin(x) * std::sin(y + pi3)
         + std::sin(y) * std::sin(z + pi3)
         + std::sin(z) * std::sin(x + pi3);
  }

  // F-RD surface (Schoen F-surface / face-centred reticular distortion, 1970)
  // f = 4*(cos(x)cos(y) + cos(y)cos(z) + cos(z)cos(x))
  //   - (cos(2x) + cos(2y) + cos(2z)) - 4
  // Good isotropic mechanical properties
  static double frd(double x, double y, double z) {
    return 4.0  * (std::cos(x) * std::cos(y)
                 + std::cos(y) * std::cos(z)
                 + std::cos(z) * std::cos(x))
         - (std::cos(2.0 * x) + std::cos(2.0 * y) + std::cos(2.0 * z))
         - 4.0;
  }

  /* ---------------------------------------------------------------------------
     Dispatcher: evaluate any TPMS type by name string
     Matches ASLI's existing string-based type selection convention
     ------------------------------------------------------------------------- */
  static double evaluate(const std::string& type,
                         double x, double y, double z)
  {
    // --- Original ASLI types ---
    if      (type == "gyroid")        return gyroid(x, y, z);
    else if (type == "schwarzP"  ||
             type == "schwartz_p"||
             type == "primitive")     return schwarzP(x, y, z);
    else if (type == "schwarzD"  ||
             type == "schwartz_d"||
             type == "diamond")       return schwarzD(x, y, z);

    // --- Extended types ---
    else if (type == "neovius")       return neovius(x, y, z);
    else if (type == "iwp"       ||
             type == "IWP"       ||
             type == "i-wp")          return iwp(x, y, z);
    else if (type == "lidinoid")      return lidinoid(x, y, z);
    else if (type == "splitP"    ||
             type == "split_p"   ||
             type == "split-p")       return splitP(x, y, z);
    else if (type == "fischerKochS"||
             type == "fisher_koch_s"||
             type == "fischer-koch") return fischerKochS(x, y, z);
    else if (type == "schwarzH"  ||
             type == "schwartz_h"||
             type == "hexagonal")    return schwarzH(x, y, z);
    else if (type == "frd"       ||
             type == "FRD"       ||
             type == "f-rd")         return frd(x, y, z);

    else {
      throw std::invalid_argument(
        "Unknown TPMS type: '" + type + "'.\n"
        "Supported types: gyroid, schwarzP, schwarzD, neovius, iwp,\n"
        "                 lidinoid, splitP, fischerKochS, schwarzH, frd"
      );
    }
  }

  /* ---------------------------------------------------------------------------
     Scaled evaluation: applies the 2*pi/cellSize scaling used by ASLI
     coords are raw spatial coordinates (mm), cellSize is in mm
     ------------------------------------------------------------------------- */
  static double evaluateScaled(const std::string& type,
                                double xRaw, double yRaw, double zRaw,
                                double cellSize)
  {
    const double scale = 2.0 * M_PI / cellSize;
    return evaluate(type, xRaw * scale, yRaw * scale, zRaw * scale);
  }
};

#endif // TPMS_H
