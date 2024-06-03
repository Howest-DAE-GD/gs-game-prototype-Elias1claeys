#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "iostream"
#include "Texture.h"
#include "SVGParser.h"

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
	m_Level = level0;
	m_LoseTex = new Texture("You lose", "dpcomic.ttf", 100, Color4f(0.f, 0.f, 0.f, 1.f));
	m_direction = standard;
	CreateLevel();
}

void Game::Cleanup( )
{
	m_Vertices.clear();

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

			switch (m_Level)
			{
			case Game::level0:
				m_Level = level1;		
				break;
			case Game::level1:
				m_Level = level2;
				break;
			case Game::level2:
				break;
			default:
				break;
			}

			CreateLevel();
		}
	}
}

void Game::Draw() const
{
	DrawMap();

	m_Text->Draw(Point2f(GetViewPort().width / 2 - m_Text->GetWidth() / 2, GetViewPort().height - 75.f));

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
	if (!m_lose && !m_GreenSquareInfo.moving && !m_RedSquareInfo.moving)
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
		m_RedSquareInfo.moving = false;
		m_GreenSquareInfo.moving = false;
		CreateLevel();
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

	for (int i = 0; i < m_Vertices.size(); i++)
	{
		for (int j = 0; j < m_Vertices[i].size() - 1; j++)
		{
			utils::DrawLine(m_Vertices[i][j], m_Vertices[i][j + 1], 5.f);
		}

		utils::DrawLine(m_Vertices[i][0], m_Vertices[i][m_Vertices[i].size() - 1], 5.f);
	}
}

void Game::CreateLevel()
{
	m_Vertices.clear();
	m_RedSquareInfo.moving = false;
	m_GreenSquareInfo.moving = false;

	switch (m_Level)
	{
	case Game::level0:
		SVGParser::GetVerticesFromSvgFile("level0.svg", m_Vertices);
		m_Text = new Texture("level0", "dpcomic.ttf", 60, Color4f(0.f, 0.f, 0.f, 1.f));
		m_RedSquareInfo.Square = Rectf(GetViewPort().width - 155.f, GetViewPort().height - 155.f, 50.f, 50.f);
		m_GreenSquareInfo.Square = Rectf(105.f, GetViewPort().height - 155.f, 50.f, 50.f);
		m_BlueSquare = Rectf(GetViewPort().width / 2 - 50.f, GetViewPort().bottom + 120.f, 100.f, 100.f);
		m_Velocity = Vector2f( 500.f, 500.f );
		break;

	case Game::level1:
		SVGParser::GetVerticesFromSvgFile("level1.svg", m_Vertices);
		m_Text = new Texture("level1", "dpcomic.ttf", 60, Color4f(0.f, 0.f, 0.f, 1.f));
		m_GreenSquareInfo.Square = Rectf(120.f, GetViewPort().height - 170.f, 25.f, 25.f);
		m_RedSquareInfo.Square = Rectf(GetViewPort().width - 200.f, GetViewPort().height - 170.f, 25.f, 25.f);
		m_BlueSquare = Rectf(120.f, GetViewPort().bottom + 105.f, 175.f, 50.f);
		m_Velocity = Vector2f(500.f, 500.f);
		break;
	case Game::level2:
		SVGParser::GetVerticesFromSvgFile("level2.svg", m_Vertices);
		m_Text = new Texture("level2", "dpcomic.ttf", 60, Color4f(0.f, 0.f, 0.f, 1.f));
		m_GreenSquareInfo.Square = Rectf(GetViewPort().width / 2 + 150.f, GetViewPort().height / 2 - 90.f, 15.f, 15.f);
		m_RedSquareInfo.Square = Rectf(GetViewPort().width/ 2 - 125.f, GetViewPort().height / 2 - 90.f, 15.f, 15.f);
		m_BlueSquare = Rectf(115.f, GetViewPort().height - 100.f, 50, 50.f);
		m_Velocity = Vector2f(100.f, 100.f);
		break;
	default:
		break;
	}
}

Game::SquareInfo Game::UpdateSquares(float elapsedsec, Rectf Square, bool moving)
{
	Vector2f velocity{ m_Velocity };

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

	for (int i = 0; i < m_Vertices.size(); i++)
	{
		if (moving)
		{
			switch (m_direction)
			{
			case Game::left:
				if (utils::Raycast(m_Vertices[i], raystartLeft, rayEndLeft, result))
				{
					velocity.x = 0;
					moving = false;
					Square.left += 5.f;
				}
				else
				{
					Square.left -= velocity.x * elapsedsec;
				}
				break;
			case Game::right:
				if (utils::Raycast(m_Vertices[i], rayStartRight, rayEndRight, result))
				{
					velocity.x = 0;
					moving = false;
					Square.left -= 5.f;
				}
				else
				{
					Square.left += velocity.x * elapsedsec;
				}
				break;
			case Game::up:
				if (utils::Raycast(m_Vertices[i], raystartUp, rayEndUp, result))
				{
					velocity.y = 0;
					moving = false;
					Square.bottom -= 5.f;
				}
				else
				{
					Square.bottom += velocity.y * elapsedsec;
				}
				break;
			case Game::down:
				if (utils::Raycast(m_Vertices[i], raystartDown, rayEndDown, result))
				{
					velocity.y = 0;
					moving = false;
					Square.bottom += 5.f;
				}
				else
				{
					Square.bottom -= velocity.y * elapsedsec;
				}
				break;
			}
		}
	}

	return{ moving, Square };
}
