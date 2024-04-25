#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "iostream"

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	UpdateMap();
	RotateMap();
	m_GreenBallPosition = UpdateBall(elapsedSec, m_GreenBallPosition);
	m_RedBallPosition = UpdateBall(elapsedSec, m_RedBallPosition);
}

void Game::Draw() const
{
	DrawMap();

	utils::SetColor(Color4f(0.1f, 1.f, 0.1f, 1.f));
	utils::FillEllipse(m_GreenBallPosition, 20, 20);
	utils::SetColor(Color4f(1.f, 0.f, 0.1f, 1.f));
	utils::FillEllipse(m_RedBallPosition, 20, 20);
}
void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{

}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{

}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::DrawMap() const
{
	ClearBackground();
	utils::SetColor(Color4f(0.f, 0.f, 0.f, 1.f));

	for (int i = 0; i < m_Platform.size() - 1; i++)
	{
		utils::DrawLine(m_Platform[i], m_Platform[i + 1], 5.f);
	}
	utils::DrawLine(m_Platform[0], m_Platform[m_Platform.size() - 1], 5.f);

	//utils::DrawEllipse(Point2f(centerX, CenterY), radius, radius);
	//utils::DrawRect(GetViewPort().width / 2 - 325.f, GetViewPort().height / 2 - radius, radius * 2, radius * 2);
}

void Game::UpdateMap()
{
	m_Platform.clear();
	float radius{ 325.f };
	const float pi{ 3.1415f };
	const float centerX{ GetViewPort().width / 2 };
	const float CenterY{ GetViewPort().height / 2 };
	float angle{ 45 * 0.0174532925f };
	float angle2{ 135 * 0.0174532925f };
	float angleInc{ 2 * pi / 360 };

	angle = angle + angleInc * m_Movement.x;
	angle2 = angle2 + angleInc * m_Movement.x;

	Point2f point1{ float(centerX + cos(angle) * -radius), float(CenterY + sin(angle) * -radius) };
	Point2f point2{ float(centerX + cos(angle2) * radius), float(CenterY + sin(angle2) * radius) };
	Point2f point3{ float(centerX + cos(angle) * radius), float(CenterY + sin(angle) * radius) };
	Point2f point4{ float(centerX + cos(angle2) * -radius), float(CenterY + sin(angle2) * -radius) };

	m_Platform.push_back(point1);
	m_Platform.push_back(point2);
	m_Platform.push_back(point3);
	m_Platform.push_back(point4);
}

Point2f Game::UpdateBall(float elapsedSec, Point2f Position)
{
	Rectf bounds{ Position.x - 20, Position.y - 20, 40, 40 };

	Point2f raystart{ bounds.left, bounds.bottom + bounds.height };
	Point2f rayEnd{ bounds.left, bounds.bottom - 5.f};

	Point2f raystart2{ bounds.left + bounds.width, bounds.bottom + bounds.height };
	Point2f rayEnd2{ bounds.left + bounds.width, bounds.bottom - 5.f};
	utils::HitInfo result{};

	if (!utils::IsPointInPolygon(Position, m_Platform))
	{
		Position.y += 100.f;
	}
	else
	{
		if (utils::Raycast(m_Platform, raystart, rayEnd, result) && utils::Raycast(m_Platform, raystart2, rayEnd2, result))
		{
			m_Velocity.y = 0;
		}
		else if (utils::Raycast(m_Platform, raystart, rayEnd, result) && !utils::Raycast(m_Platform, raystart2, rayEnd2, result))
		{
			Position.x += m_Velocity.x * elapsedSec;
			Position.y -= m_Velocity.y * elapsedSec;
		}
		else if (!utils::Raycast(m_Platform, raystart, rayEnd, result) && utils::Raycast(m_Platform, raystart2, rayEnd2, result))
		{
			Position.x -= m_Velocity.x * elapsedSec;
			Position.y -= m_Velocity.y * elapsedSec;
		}
		else
		{
			m_Velocity.y = 100.f;
			Position.y -= m_Velocity.y * elapsedSec;
		}
	}
	return Position;
}

void Game::RotateMap()
{
	const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);
	const float speed{ 0.5f };

	if (keyboard_state_array[SDL_SCANCODE_LEFT])
	{
		m_Movement.x += speed;
		m_Movement.y += speed;
	}
	if (keyboard_state_array[SDL_SCANCODE_RIGHT])
	{
		m_Movement.x -= speed;
		m_Movement.y -= speed;
	}

	if (m_Movement.x >= 360.f)
	{
		m_Movement = Point2f{ 0, 0 };
	}
}
