///@author Mekhail Muller

#ifndef __MiddleStage_h__
#define __MiddleStage_h__

#include "WarStage.h"

// class WarStage;
class MiddleStage;

class MiddleStage : public WarStage
{

public:
        /**
         * @brief returns warstage via int = 1
         */
        int getWarStage();

        /**
         * @brief Construct a new Middle Stage object
         *
         */
        MiddleStage();

        /**
         * @brief Destroy the Middle Stage object
         *
         */
        ~MiddleStage();

        /**
         * @brief Return a deep copy of the Middle Stage object
         *
         * @return MiddleStage*
         */
        MiddleStage *clone();
};

#endif
