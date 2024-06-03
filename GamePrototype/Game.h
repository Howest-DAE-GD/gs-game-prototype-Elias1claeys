#pragma once
#include "BaseGame.h"
#include "vector"
#include "Vector2f.h"
#include "Texture.h"
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
	struct SquareInfo
	{
		bool moving{};
		Rectf Square{};
	};

	void Initialize();
	void Cleanup();
	void ClearBackground() const;
	void DrawMap() const;
	void CreateLevel();
	SquareInfo UpdateSquares(float elapsedsec, Rectf Square, bool moving);


	Rectf m_BlueSquare{GetViewPort().width / 2 - 50.f, GetViewPort().bottom + 105.f, 100.f, 100.f};
	std::vector<Point2f> m_Platform{};
	bool m_Moving{ false };
	bool m_lose{ false };

	enum Direction
	{
		left,
		right,
		up,
		down,
		standard,
	};

	enum levels
	{
		level0,
		level1,
		level2,
	};

	std::vector<std::vector<Point2f>> m_Vertices;
	SquareInfo m_RedSquareInfo{false, Rectf(GetViewPort().width - 155.f, GetViewPort().height - 155.f, 50.f, 50.f)};
	SquareInfo m_GreenSquareInfo{false, Rectf(105.f, GetViewPort().height - 155.f, 50.f, 50.f)};
	Direction m_direction{};
	levels m_Level;
	Texture* m_Text;
	Texture* m_LoseTex;
	Vector2f m_Velocity{ 500.f, 500.f };
};