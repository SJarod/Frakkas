#include "player_animation.hpp"

KK_COMPONENT_IMPL_BEGIN(PlayerAnimation)

KK_COMPONENT_IMPL_END

void PlayerAnimation::SetState(const EAnimState i_state)
{
	if (!ad->animGraph.player.IsWaiting() && state == i_state)
		return;

	state = i_state;

	switch (state)
	{
	case EAnimState::IDLE:
		ad->animGraph.PlayAnimation("Hero_Idle_1.fbx_mixamo.com", true);
		break;
	case EAnimState::WALK:
		ad->animGraph.PlayAnimation("Hero_Walk.fbx_mixamo.com", true);
		break;
	case EAnimState::RUN:
		ad->animGraph.PlayAnimation("Hero_Run.fbx_mixamo.com", true);
		break;
	default:
		break;
	}
}

void PlayerAnimation::OnStart()
{
	ad = owner.get()->GetComponent<AnimatedDraw>();
	SetState(EAnimState::IDLE);
}

void PlayerAnimation::OnUpdate()
{
	float xTranslation = Inputs::GetAxis("horizontal");
	float zTranslation = Inputs::GetAxis("forward");

	if (ad)
	{
		if (Inputs::IsPressed(Game::EButton::MOUSE_LEFT))
		{
			int i = attack++;
			i = Maths::Modulo(i, 5) + 1;
			Log::Info(i);
			ad->animGraph.PlayAnimation("Hero_Slash_" + std::to_string(i) + ".fbx_mixamo.com");
		}

		Vector3 dir = Vector3(xTranslation, 0.f, -zTranslation);
		if (dir.Length() == 0.f)
		{
			SetState(EAnimState::IDLE);
		}
		else
		{
			if (Inputs::IsDown(Game::EButton::LEFT_SHIFT))
				SetState(EAnimState::RUN);
			else
				SetState(EAnimState::WALK);
		}
	}
}