//
// Created by Christian Sejersen on 21/11/2023.
//

#pragma once;

class AttackTableGenerator {
public:
    void loadAttackTables();
private:
    U64 generateBlockers(int,int,U64) const;
    void generateBishopAttackMasks();
    void generateRookAttackMasks();
    U64 rookAttacksOnTheFly(int square, U64 blockers);
    U64 bishopAttacksOnTheFly(int square, U64 blockers);
};


#endif //BBCHESS_ENGINE_ATTACKTABLEGENERATOR_H
