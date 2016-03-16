

#include "cubelib/cubelib.h"

#include <ostream>
#include <iostream>
#include <fstream>



/**
 * Spits out @c cubelib_cnr_adj_cnrs as raw constants, for use in opencl.
 */
void generate_cnr_adj_cnrs_constants(std::ostream& out)
{
    out << "#define CUBELIB_CNR_ADJ_CNRS \\" << std::endl;
    out << "    { \\" << std::endl;

    for (std::size_t corneri = 0; corneri < 8; ++corneri)
    {
        cubelib_corner_t corner = cubelib_all_corners[corneri];

        out << "        " << (corneri == 0 ? "  " : ", ") << "{ \\" << std::endl;
        for (std::size_t i = 0; i < 3; ++i)
        {
            cubelib_corner_t adj_corner = cubelib_calc_cnr_adj_cnr(corner, i);
            // cubelib_corner_t adj_corner = cubelib_cnr_adj_cnrs[cubelib_get_corner_index(corner)][i];

            out << "            " << (i == 0 ? "  " : ", ") << "(corner_t){" << adj_corner.value << "}" << " \\" << std::endl;
        }
        out << "        } \\" << std::endl;
    }
    out << "    }" << std::endl;
}





/**
 * Spits out @c dir_adj_cnrs as raw constants, for use in opencl.
 */
void generate_dir_adj_cnrs_constants(std::ostream& out)
{
    out << "#define CUBELIB_FACE_ADJ_CNRS \\" << std::endl;
    out << "    { \\" << std::endl;

    for (std::size_t facei = 0; facei < 6; ++facei)
    {
        cubelib_face_t face = cubelib_all_faces[facei];

        out << "        " << (facei == 0 ? "  " : ", ") << "{ \\" << std::endl;
        for (std::size_t i = 0; i < 4; ++i)
        {
            
            cubelib_corner_t adj_corner = cubelib_calc_corner_on_face(face, i);

            out << "            " << (i == 0 ? "  " : ", ") << "(corner_t){" << adj_corner.value << "}" << " \\" << std::endl;
        }
        out << "        } \\" << std::endl;
    }
    out << "    }" << std::endl;
}


/**
 * Spits out @c (pos/neg)(x/y)face as raw constants, for use in opencl.
 */
void generate_face_constants(std::ostream& out)
{


    out << "#define POSXFACE (cubelib_face_t){" << posxface.value << "}" << std::endl;
    out << "#define NEGXFACE (cubelib_face_t){" << negxface.value << "}" << std::endl;
    out << "#define POSYFACE (cubelib_face_t){" << posyface.value << "}" << std::endl;
    out << "#define NEGYFACE (cubelib_face_t){" << negyface.value << "}" << std::endl;
    out << "#define POSZFACE (cubelib_face_t){" << poszface.value << "}" << std::endl;
    out << "#define NEGZFACE (cubelib_face_t){" << negzface.value << "}" << std::endl;

}




int main(int argc, char** argv){

    //std::ofstream out("cubelib.gen.h");
    std::ostream& out = std::cout;

    out << "///Generated via cubelib.cpp" << std::endl;
    out << "///" << std::endl;
    out << "///" << std::endl;
    out << "///DO NOT MODIFY" << std::endl;
    out << "///MODIFICATIONS WILL BE OVERWRITTEN WHEN THIS FILE IS REGENERATED" << std::endl;
    out << "///EDIT THE GENERATOR IF NECESSARY" << std::endl;

    for (std::size_t i = 0; i < 5; ++i)
        out << std::endl;


    generate_cnr_adj_cnrs_constants(out);

    for (std::size_t i = 0; i < 5; ++i)
        out << std::endl;

    generate_dir_adj_cnrs_constants(out);

    for (std::size_t i = 0; i < 5; ++i)
        out << std::endl;

    generate_face_constants(out);

    for (std::size_t i = 0; i < 5; ++i)
        out << std::endl;

    return 0;
}




