#pragma once

enum Output
{
	Movement,
	Jump,
	//Run,
	Up,

	OUTPUT_MAX
};

namespace Activation
{
	const double MoveLeftMin = 0.0;
	const double MoveLeftMax = 0.4;
	const double MoveRightMin = 0.6;
	const double MoveRightMax = 1.0;
	const double JumpMin = 0.5;
	const double JumpMax = 1.0;
	const double RunMin = 0.5;
	const double RunMax = 1.0;
	const double UpMin = 0.5;
	const double UpMax = 1.0;
}
