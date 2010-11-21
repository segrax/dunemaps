
class cMinimap {
private:
	word			 _mapX, _mapY;
	SDL_Surface		*_surfaceAreaFrame;	

public:

					 cMinimap( );
					~cMinimap();


			bool	 positionUpdate();

	virtual void	 draw( cVideoSurface *pSurface );
			void	 drawPiece( word pMapIndex, cVideoSurface *pSurface );
			void	 drawAreaFrame( cVideoSurface *pSurface );

			void	 buttonClear();
	virtual void	 buttonPressLeft( size_t pX, size_t pY );
	virtual void	 buttonPressRight(size_t pX, size_t pY );
	virtual void	 buttonReleaseLeft(size_t pX, size_t pY );
	virtual void	 buttonReleaseRight(size_t pX, size_t pY );

};
