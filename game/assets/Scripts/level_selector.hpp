#pragma once

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(LevelSelector);

        void OnEnable() override;

    private:
        static constexpr float minimumLoadingTime = 5.f;

        // SCENES PATH
        static constexpr char forestScenePath[] = "game/assets/Scenes/forest.kk";
        static constexpr char desertScenePath[] = "game/assets/Scenes/desert.kk";
        static constexpr char snowScenePath[] = "game/assets/Scenes/snow.kk";
        static constexpr char darkScenePath[] = "game/assets/Scenes/dark.kk";

        // SCENES LOADING SCREEN
        // Forest
        static constexpr char forestScreen1[] = "game/assets/Textures/LoadingScreens/forest_heart.png";
        static constexpr char forestScreen2[] = "game/assets/Textures/LoadingScreens/forest_player_2.png";
        static constexpr char forestScreen4[] = "game/assets/Textures/LoadingScreens/forest_skeleton_1.png";
        // Snow
        static constexpr char snowScreen1[] = "game/assets/Textures/LoadingScreens/snow_fight_1.png";
        static constexpr char snowScreen3[] = "game/assets/Textures/LoadingScreens/snow_portal.png";
        static constexpr char snowScreen4[] = "game/assets/Textures/LoadingScreens/snow_skeleton_1.png";
        // Desert
        static constexpr char desertScreen1[] = "game/assets/Textures/LoadingScreens/desert_fight.png";
        static constexpr char desertScreen2[] = "game/assets/Textures/LoadingScreens/desert_golem.png";
        static constexpr char desertScreen3[] = "game/assets/Textures/LoadingScreens/desert_player_1.png";
        static constexpr char desertScreen4[] = "game/assets/Textures/LoadingScreens/desert_skeleton_1.png";
        // Dark
        static constexpr char darkScreen1[] = "game/assets/Textures/LoadingScreens/dark_fight.png";
        static constexpr char darkScreen2[] = "game/assets/Textures/LoadingScreens/dark_golem_1.png";
        static constexpr char darkScreen3[] = "game/assets/Textures/LoadingScreens/dark_skeleton_1.png";
        static constexpr char darkScreen4[] = "game/assets/Textures/LoadingScreens/dark_skeleton_2.png";
        // TIPS
        static constexpr char itemTips[] = "Sometimes, enemies can drop life item."
                                           "\nIt heals you 17% of your health, and gives you the smile =)";
        static constexpr char itemRateTips[] = "The lower your health is, the greater is your chance to find health item.";
        static constexpr char difficultyTips[] = "The more level you succeeded, The more the enemies are strong.";
        static constexpr char attackComboTips[] = "Your damage increases when you process combo attack, but the third attack stuns you.";
        static constexpr char comboScoreTips[] = "Your combo factor increase when you kill 5 enemies in a short period! Yaaah!";
        static constexpr char stupidDeathTips[] = "Don't die, if you want to live.";
        static constexpr char armorTruthTips[] = "You have an armor, yes."
                                                 "\nBut did you know that it is in paper-craft ?";
        static constexpr char matrixTips[] = "Transpose(Inverse(Matrix)) = Inverse(Transpose(Matrix))";
        static constexpr char bnTips[] = "BN stands for Biscuiterie Nantaise.";
        static constexpr char weaponTips[] = "Care not to drop your weapon.";
        static constexpr char stopTips[] = "Press 'START' and return to the menu if you need to stop playing"
                                           "\nbecause you are asked to set the table.";
        static constexpr char bossTips[] = "You have 0.0001% chance to find the lord of the Dark world, AkA the final boss.";
        static constexpr char frakkasTips[] = "'Frakkas' name definitely comes from 'fracasser' in french."
                                            "\nIt means the engine breaks everything ! In the good ways...";
        static constexpr char forestTips[] = "In the green lands, the wind behind your back makes you run faster!";
        static constexpr char snowTips[] = "at Frozen kingdom, you can't run fast due to snow. Well, who needs to run though?";
        static constexpr char desertTips[] = "The blinding sun of the desert gives speed to golems. Beware of their attacks!";
        static constexpr char darkTips[] = "The Dark world is the nursery of golems. Hope your sword can slash rocks...";
        static constexpr char gamepadTips[] = "We hope you're playing with a controller. Gamepad vibrations are so satisfying!";
        static constexpr char goldTips[] = "9th June at 7:16 PM is the perfect time to have a toast.";

        std::vector<const char*> scenePaths = { forestScenePath, desertScenePath, snowScenePath, darkScenePath };

        std::vector<const char*> tips = { itemTips, itemRateTips, attackComboTips, comboScoreTips, stupidDeathTips,
                                          armorTruthTips, matrixTips, difficultyTips, bnTips, weaponTips, stopTips, bossTips,
                                          frakkasTips, forestTips, snowTips, desertTips, darkTips, gamepadTips, goldTips
        };

        std::vector<const char*> forestSceneScreens = { forestScreen1, forestScreen2, forestScreen4};
        std::vector<const char*> snowSceneScreens = { snowScreen1, snowScreen3, snowScreen4};
        std::vector<const char*> desertSceneScreens = { desertScreen1, desertScreen2, desertScreen3, desertScreen4};
        std::vector<const char*> darkSceneScreens = { darkScreen1, darkScreen2, darkScreen3, darkScreen4};

    KK_COMPONENT_END
}
