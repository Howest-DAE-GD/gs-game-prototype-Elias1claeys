#pragma once
#include "BaseGame.h"
#include "vector"
#include "Vector2f.h"
class Game : public BaseGame
{
public:
	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update(float elapsedSec) override;
	void Draw() const override;

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup();
	void ClearBackground() const;
	void DrawMap() const;
	void UpdateMap();
	Point2f UpdateBall(float elapsedsec, Point2f Position);
	void RotateMap();

	Point2f m_Movement{ 0.f, 0.f };
	Point2f m_GreenBallPosition{ 300.f, 500.f };
	Point2f m_RedBallPosition{ 700.f, 500.f };
	Vector2f m_Velocity{ 200.f, 100.f };
	std::vector<Point2f> m_Platform{};
};