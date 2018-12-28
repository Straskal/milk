#include "Animator.h"

#include "../components/Sprite.h"
#include "../core/Actor.h"
#include "../utilities/Texture.h"

const ComponentType Animator::type = ANIMATOR;

Animator::Animator(Actor& actor)
	: ActorComponent::ActorComponent(actor)
{
	currentAnimation_ = 0;
	timeBetweenFrames_ = 0.1f;
	rows_ = 0;
	columns_ = 0;
	frameWidth_ = 0;
	frameWidth_ = 0;
	paused_ = false;

	currentAnimation_ = nullptr;

	timer_ = Timer();
}

Animator::~Animator()
{
}

void Animator::init()
{
	sprite_ = actor_.getComponent<Sprite>();

	frameWidth_ = sprite_->texture()->getWidth() / columns_;
	frameHeight_ = sprite_->texture()->getHeight() / rows_;

	setAnimation("main");
}

void Animator::rows(int rows)
{
	rows_ = rows;
}

void Animator::columns(int columns)
{
	columns_ = columns;
}

void Animator::togglePaused()
{
	paused_ = !paused_;
}

void Animator::addAnimation(const std::string& name, std::initializer_list<int> f)
{
	animations_.insert(std::make_pair(name, std::unique_ptr<Animation>(new Animation(name, f))));
}

void Animator::addAnimation(const std::string& name, std::vector<int> f)
{
	animations_.insert(std::make_pair(name, std::unique_ptr<Animation>(new Animation(name, f))));
}

void Animator::setAnimation(const std::string& name)
{
	if (currentAnimation_ != nullptr && currentAnimation_->name == name)
		return;

	currentAnimation_ = animations_.at(name).get();
	currentFrame_ = 0;

	timer_.start();

	int row = (int)((float)currentAnimation_->frames[currentFrame_] / columns_);
	int column = currentAnimation_->frames[currentFrame_] % columns_;

	sprite_->sourceRect(column * frameWidth_, row * frameHeight_, frameWidth_, frameHeight_);
}

void Animator::update()
{
	if (currentAnimation_ != nullptr && !paused_) 
	{
		auto t = timer_.getTicks() / 1000.0f;

		if (timer_.getTicks() / 1000.0f > timeBetweenFrames_) 
		{
			timer_.start();

			currentFrame_++;

			if (currentFrame_ > currentAnimation_->size - 1) 
			{
				currentFrame_ = 0;
			}

			int row = (int)((float)currentAnimation_->frames[currentFrame_] / columns_);
			int column = currentAnimation_->frames[currentFrame_] % columns_;

			sprite_->sourceRect(column * frameWidth_, row * frameHeight_, frameWidth_, frameHeight_);
		}
	}
}
