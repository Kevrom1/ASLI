/* =============================================================================
   Copyright (C) 2022 KU Leuven
   ASLI (A Simple Lattice Infiller) - Extended version with additional TPMS types
   Original: https://github.com/tpms-lattice/ASLI
   License: GNU Affero General Public License

   TPMS.cpp — Implementation and validation helpers
   The main function bodies live as inline statics in TPMS.h.
   This file provides:
     - A listing of all supported TPMS names (for validation / GUI population)
     - A description map for user-facing documentation
   ============================================================================= */

#include "TPMS.h"
#include <map>
#include <vector>
#include <iostream>

/* ---------------------------------------------------------------------------
   Returns the canonical name list of all supported TPMS types.
   Preserves original ASLI types first, then extended types.
   ------------------------------------------------------------------------- */
std::vector<std::string> TPMS_supportedTypes() {
  return {
    // --- Original ASLI types ---
    "gyroid",
    "schwarzP",
    "schwarzD",

    // --- Extended types ---
    "neovius",
    "iwp",
    "lidinoid",
    "splitP",
    "fischerKochS",
    "schwarzH",
    "frd"
  };
}

/* ---------------------------------------------------------------------------
   Returns a human-readable description for each TPMS type.
   Used by QASLI GUI tooltips and by the --list-types CLI flag.
   ------------------------------------------------------------------------- */
std::map<std::string, std::string> TPMS_descriptions() {
  return {
    // --- Original ---
    {"gyroid",
      "Gyroid (Schoen 1970). Equation: sin(x)cos(y)+sin(y)cos(z)+sin(z)cos(x). "
      "No planes of symmetry. Excellent for heat exchangers and biomedical scaffolds."},

    {"schwarzP",
      "Schwarz Primitive (Schwarz 1865). Equation: cos(x)+cos(y)+cos(z). "
      "Cubic symmetry. High permeability, often used in tissue engineering."},

    {"schwarzD",
      "Schwarz Diamond (Schwarz 1865). Equation: cos(x)cos(y)cos(z)-sin(x)sin(y)sin(z). "
      "High stiffness-to-weight ratio. Popular for bone scaffold applications."},

    // --- Extended ---
    {"neovius",
      "Neovius surface (Neovius 1883). Equation: 3(cos(x)+cos(y)+cos(z))+4cos(x)cos(y)cos(z). "
      "Higher-genus surface with a highly interconnected channel network. "
      "Highest energy absorption among common TPMS types."},

    {"iwp",
      "Schoen I-WP / Wrapped Package (Schoen 1970). "
      "Equation: 2(cos(x)cos(y)+cos(y)cos(z)+cos(z)cos(x))-(cos(2x)+cos(2y)+cos(2z)). "
      "Very high surface-area-to-volume ratio. Ideal for heat exchangers."},

    {"lidinoid",
      "Lidinoid (Lidin & Larsson 1990). Chiral surface — no planes of mirror symmetry. "
      "Continuous deformation of the gyroid family. Unique anisotropic mechanical response."},

    {"splitP",
      "Split-P (Schoen 1970). Variant of the Schwarz-P with split topology. "
      "Combines properties of the P and D surfaces."},

    {"fischerKochS",
      "Fischer-Koch S surface (Fischer & Koch 1987). "
      "Equation: cos(2x)sin(y)cos(z)+cos(x)cos(2y)sin(z)+sin(x)cos(y)cos(2z). "
      "Excellent mechanical continuity. Well-suited for graded biomedical implants."},

    {"schwarzH",
      "Schwarz Hexagonal / H-surface (Schwarz 1865). Hexagonal (not cubic) symmetry group. "
      "Unique directional transport properties along the hexagonal axis."},

    {"frd",
      "Schoen F-RD / Face-centred Reticular Distortion (Schoen 1970). "
      "Equation: 4(cos(x)cos(y)+cos(y)cos(z)+cos(z)cos(x))-(cos(2x)+cos(2y)+cos(2z))-4. "
      "Good isotropic stiffness. Suitable for lightweight structural infills."}
  };
}

/* ---------------------------------------------------------------------------
   Validates a TPMS type string. Returns true if it is recognised.
   Mirrors the style of parameter validation already in ASLI.
   ------------------------------------------------------------------------- */
bool TPMS_isValidType(const std::string& type) {
  try {
    // Probe evaluation at origin — if it throws the type is unknown
    TPMS::evaluate(type, 0.0, 0.0, 0.0);
    return true;
  } catch (const std::invalid_argument&) {
    return false;
  }
}

/* ---------------------------------------------------------------------------
   Prints all supported TPMS types to stdout.
   Called when the user passes --list-types on the command line.
   ------------------------------------------------------------------------- */
void TPMS_printSupportedTypes() {
  auto names = TPMS_supportedTypes();
  auto descs = TPMS_descriptions();

  std::cout << "\nSupported TPMS unit cell types\n"
            << "==============================\n";

  // Separator between original and extended
  bool printedSep = false;
  std::vector<std::string> originalTypes = {"gyroid", "schwarzP", "schwarzD"};

  for (const auto& name : names) {
    bool isOriginal = (std::find(originalTypes.begin(),
                                 originalTypes.end(), name) != originalTypes.end());
    if (!isOriginal && !printedSep) {
      std::cout << "\n  -- Extended types (added in this version) --\n\n";
      printedSep = true;
    }

    std::cout << "  " << name << "\n";
    if (descs.count(name)) {
      // Word-wrap description at 70 chars, indented
      std::string d = descs.at(name);
      std::cout << "    " << d.substr(0, 70) << "\n";
      if (d.size() > 70)
        std::cout << "    " << d.substr(70) << "\n";
    }
    std::cout << "\n";
  }
}
