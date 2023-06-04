#pragma once
#include "DrawComponent.h"

class ParticleComponent : public DrawComponent {
public:
	ParticleComponent();
	~ParticleComponent();

	void draw() override;
};