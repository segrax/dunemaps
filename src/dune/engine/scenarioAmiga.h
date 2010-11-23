class cScenarioAmiga : public cScenario {
private:

	string						 get_string( byte **pBuffer );
	word						 get_word( byte **pBuffer );
	string						 get_numberstring( byte **pBuffer );

	void						 load_Basic( byte pKeyID, byte **pBuffer );
	void						 load_Map( byte keyID, byte **pBuffer );
	void						 load_House( byte sectionID, byte keyID, byte **pBuffer );
	void						 load_Units( byte keyID, byte **pBuffer );
	void						 load_Structures( byte keyID, byte **pBuffer );
	void						 load_Teams( byte keyID, byte **pBuffer );
	void						 load_Choam( byte keyID, byte **pBuffer );
	void						 load_Reinforcements( byte keyID, byte **pBuffer );
	
public:
	
	void						 iniLoad( string pFile, bool pLocalFile );
	//void						 iniSave( string pFile );
};
