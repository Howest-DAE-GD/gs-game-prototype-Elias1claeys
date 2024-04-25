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

void Game::Draw( ) const
{
	float radius{ 325.f };
	const float pi{ 3.1415f };
	const float centerX{ GetViewPort().width / 2 };
	const float CenterY{ GetViewPort().height / 2 };
	float angle{ 45 * 0.0174532925 };
	float angle2{ 135 * 0.0174532925 };
	float angleInc{ 2 * pi / 360 };

	angle = angle + angleInc * m_Movement.x;
	angle2 = angle2 + angleInc * m_Movement.x;

	Point2f point1{ float(centerX + cos(angle) * -radius), float(CenterY + sin(angle) * -radius) };
	Point2f point2{ float(centerX + cos(angle2) * radius), float(CenterY + sin(angle2) * radius) };
	Point2f point3{ float(centerX + cos(angle) * radius), float(CenterY + sin(angle) * radius) };
	Point2f point4{ float(centerX + cos(angle2) * -radius), float(CenterY + sin(angle2) * -radius) };

	ClearBackground( );
	utils::SetColor(Color4f(0.f, 0.f, 0.f, 1.f));
	//utils::FillEllipse(Point2f(centerX, CenterY), 10.f, 10.f);
	//utils::FillEllipse(point1, 10.f, 10.f);
	//utils::FillEllipse(point2, 10.f, 10.f);
	//utils::FillEllipse(point3, 10.f, 10.f);
	//utils::FillEllipse(point4, 10.f, 10.f);

	utils::DrawLine(point1, point2, 5.f);
	utils::DrawLine(point2, point3, 5.f);
	utils::DrawLine(point3, point4, 5.f);
	utils::DrawLine(point4, point1, 5.f);

	//utils::DrawEllipse(Point2f(centerX, CenterY), radius, radius);
	//utils::DrawRect(GetViewPort().width / 2 - 325.f, GetViewPort().height / 2 - radius, radius * 2, radius * 2);
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
