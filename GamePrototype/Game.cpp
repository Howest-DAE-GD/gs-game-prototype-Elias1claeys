#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "iostream"
#include "Texture.h"

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
	m_Text = new Texture("level0", "dpcomic.ttf", 60, Color4f( 0.f, 0.f, 0.f, 1.f ));
	m_LoseTex = new Texture("You lose", "dpcomic.ttf", 100, Color4f(0.f, 0.f, 0.f, 1.f));
	Point2f point1{GetViewPort().left + 100.f, GetViewPort().height - 100.f};
	Point2f point2{GetViewPort().width - 100.f, GetViewPort().height - 100.f};
	Point2f point3{ GetViewPort().width - 100.f, GetViewPort().bottom + 100.f};
	Point2f point4{ GetViewPort().left + 100.f, GetViewPort().bottom + 100.f };

	m_Platform.push_back(point1);
	m_Platform.push_back(point2);
	m_Platform.push_back(point3);
	m_Platform.push_back(point4);
	m_direction = standard;
}

void Game::Cleanup( )
{
	delete m_Text;
	m_Text = nullptr;

	delete m_LoseTex;
	m_LoseTex = nullptr;
}

void Game::Update( float elapsedSec )
{
	m_RedSquareInfo = UpdateSquares(elapsedSec, m_RedSquareInfo.Square, m_RedSquareInfo.moving);
	m_GreenSquareInfo = UpdateSquares(elapsedSec, m_GreenSquareInfo.Square, m_GreenSquareInfo.moving);

	if (!m_lose)
	{
		if (utils::IsOverlapping(m_RedSquareInfo.Square, m_GreenSquareInfo.Square) || utils::IsOverlapping(m_RedSquareInfo.Square, m_BlueSquare))
		{
			m_direction = standard;
			m_lose = true;
		}
		else if (utils::IsOverlapping(m_GreenSquareInfo.Square, m_BlueSquare))
		{
			m_direction = standard;
			m_lose = false;
		}
	}

}

void Game::Draw() const
{
	DrawMap();

	m_Text->Draw(Point2f(GetViewPort().width / 2 - m_Text->GetWidth() / 2, GetViewPort().height - 100.f));
	utils::SetColor(Color4f(0.1f, 1.f, 0.1f, 1.f));
	utils::FillRect(m_GreenSquareInfo.Square);
	utils::SetColor(Color4f(1.f, 0.f, 0.1f, 1.f));
	utils::FillRect(m_RedSquareInfo.Square);
	utils::SetColor(Color4f(0.f, 0.f, 1.f, 1.f));
	utils::FillRect(m_BlueSquare);

	if (m_lose)
	{
		m_LoseTex->Draw(Point2f(GetViewPort().width / 2 - m_LoseTex->GetWidth() / 2, 5.f));
	}
}
void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{

}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	if (!m_RedSquareInfo.moving && !m_GreenSquareInfo.moving)
	{
		m_RedSquareInfo.moving = true;
		m_GreenSquareInfo.moving = true;

		switch (e.keysym.sym)
		{
		case SDLK_UP:
			m_direction = up;
			break;
		case SDLK_DOWN:
			m_direction = down;
			break;
		case SDLK_LEFT:
			m_direction = left;
			break;
		case SDLK_RIGHT:
			m_direction = right;
			break;
		}
	}

	switch (e.keysym.sym)
	{
	case SDLK_r:
		m_direction = standard;
		m_lose = false;
		m_RedSquareInfo.Square = Rectf(GetViewPort().width - 155.f, GetViewPort().height - 155.f, 50.f, 50.f);
		m_RedSquareInfo.moving = false;
		m_GreenSquareInfo.Square = Rectf(105.f, GetViewPort().height - 155.f, 50.f, 50.f);
		m_GreenSquareInfo.moving = false;
		break;
	}
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
		utils::DrawLine(m_Platform[i], m_Platform[i + 1], 10.f);
	}
	utils::DrawLine(m_Platform[0], m_Platform[m_Platform.size() - 1], 10.f);
}

Game::SquareInfo Game::UpdateSquares(float elapsedsec, Rectf Square, bool moving)
{
	Vector2f velocity{ 500.f, 500.f };

	//Up
	Point2f raystartUp{ Square.left, Square.bottom + Square.height + 5.f};
	Point2f rayEndUp{ Square.left, Square.bottom + 5.f};

	//Down
	Point2f raystartDown{ Square.left, Square.bottom + Square.height - 5.f };
	Point2f rayEndDown{ Square.left, Square.bottom - 5.f };

	//Left
	Point2f raystartLeft{Square.left - 5.f, Square.bottom};
	Point2f rayEndLeft{ Square.left + Square.width - 5.f, Square.bottom + Square.height};

	//Right
	Point2f rayStartRight{ Square.left + 5.f, Square.bottom};
	Point2f rayEndRight{ Square.left + Square.width + 5.f, Square.bottom};
	utils::HitInfo result{};

	switch (m_direction)
	{
	case Game::left:
		if (utils::Raycast(m_Platform, raystartLeft, rayEndLeft, result))
		{
			velocity.x = 0;
			moving = false;
		}
		Square.left -= velocity.x * elapsedsec;
		break;
	case Game::right:
		if (utils::Raycast(m_Platform, rayStartRight, rayEndRight, result))
		{
			velocity.x = 0;
			moving = false;
		}
		Square.left += velocity.x * elapsedsec;
		break;
	case Game::up:
		if (utils::Raycast(m_Platform, raystartUp, rayEndUp, result))
		{
			velocity.y = 0;
			moving = false;
		}
		Square.bottom += velocity.y * elapsedsec;
		break;
	case Game::down:
		if (utils::Raycast(m_Platform, raystartDown, rayEndDown, result))
		{
			velocity.y = 0;
			moving = false;
		}
		Square.bottom -= velocity.y * elapsedsec;
		break;
	}

	return{moving, Square};
}
