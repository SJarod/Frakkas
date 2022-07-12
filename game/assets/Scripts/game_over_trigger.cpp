#include "ui/button.hpp"
#include "ui/text.hpp"
#include "ui/image_panel.hpp"
#include "world_data.hpp"

#include "utils/string_format.hpp"

#include "menu_script.hpp"
#include "enemy_stats.hpp"
#include "skeleton.hpp"
#include "golem.hpp"

#include "game_over_trigger.hpp"

KK_COMPONENT_IMPL_BEGIN(GameOverTrigger)

KK_COMPONENT_IMPL_END

void GameOverTrigger::OnStart()
{
    auto button = owner.get()->GetComponent<Button>();

    // Menu
    button->AddClickEvent([&]()
    {
        Time::SetTimeScale(1.f);
        World::LoadScene("game/assets/Scenes/menu.kk");
    });
}

void GameOverTrigger::OnEnable()
{
    Time::SetTimeScale(0.f);
    ComputeScore();
    World::SetInputsMode(World::InputsMode_UI);

    for (auto& comp : owner.get()->components)
        if (comp->GetID() != GetID())
            comp->enabled = true;
}

void GameOverTrigger::ComputeScore()
{
    auto skeletonStats = GetEntityContainer().FindEntityWithComponent<Skeleton>()->GetComponent<EnemyStats>();
    auto golemStats = GetEntityContainer().FindEntityWithComponent<Golem>()->GetComponent<EnemyStats>();

    if (auto worldData = World::GetWorldData<FrakkarenaWorldData>())
    {
        worldData->comboScore += worldData->levelComboScore;
        sScore = worldData->skeletonCount * skeletonStats->GetBaseScore();
        gScore = worldData->golemCount * golemStats->GetBaseScore();
        levelScore = worldData->level * 1000.f;

        const auto& texts = owner.get()->GetComponents<Text>();

        size_t maxIndex = texts.size()-1;

        texts[maxIndex-8]->text = StringFormat::GetFormat("level : ", worldData->level, " x ", 1000);// level
        texts[maxIndex-7]->text = StringFormat::GetFormat("skeleton : ", worldData->skeletonCount, " x ", skeletonStats->GetBaseScore());// skeleton
        texts[maxIndex-6]->text = StringFormat::GetFormat("golem : ", worldData->golemCount, " x ", golemStats->GetBaseScore());// golem
        texts[maxIndex-5]->text = StringFormat::GetFormat("combo bonus : ");// combo

        texts[maxIndex-4]->text = StringFormat::GetFormat(levelScore);// level
        texts[maxIndex-3]->text = StringFormat::GetFormat(sScore);// skeleton
        texts[maxIndex-2]->text = StringFormat::GetFormat(gScore);// golem
        texts[maxIndex-1]->text = StringFormat::GetFormat(worldData->comboScore);// combo

        worldData->score = levelScore + sScore + gScore + worldData->comboScore;
        texts[maxIndex]->text = StringFormat::GetFormat("Result : ", worldData->score);// total

        int gradeScore = 0;
        gradeScore += worldData->level;
        gradeScore += (worldData->skeletonCount + worldData->golemCount) / 100;
        gradeScore += worldData->comboScore / 1000;
        gradeScore /= 4;

        ImagePanel* imagePanel = owner.get()->GetComponent<ImagePanel>();

        if (gradeScore == 0) imagePanel->SetTexture("game/assets/Textures/RankingLetters_D.png");
        else if (gradeScore == 1) imagePanel->SetTexture("game/assets/Textures/RankingLetters_C.png");
        else if (gradeScore == 2) imagePanel->SetTexture("game/assets/Textures/RankingLetters_B.png");
        else if (gradeScore == 3) imagePanel->SetTexture("game/assets/Textures/RankingLetters_A.png");
        else if (gradeScore == 4) imagePanel->SetTexture("game/assets/Textures/RankingLetters_S.png");
        else if (gradeScore == 5) imagePanel->SetTexture("game/assets/Textures/RankingLetters_SS.png");
        else if (gradeScore >= 6) imagePanel->SetTexture("game/assets/Textures/RankingLetters_SSS.png");

        // Reset level count to start new game
        worldData->level = 0;
    }
}

