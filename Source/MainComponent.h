/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
class MainComponent : public OpenGLAppComponent,
	public ApplicationCommandTarget,
	public MenuBarModel,
	private Timer
{
public:
	enum CommandIDs
	{
		File_Open = 1,
		File_Save,
		File_SaveAs,
		File_Exit,
		Edit_Cut,
		Edit_Copy,
		Edit_Paste,
		Help_About,
	};

	//==============================================================================
	MainComponent();
	~MainComponent();

	//==============================================================================
	void initialise() override;
	void shutdown() override;
	void render() override;

	//==============================================================================
	void paint( Graphics& g ) override;
	void resized() override;

	//==============================================================================
	StringArray getMenuBarNames() override;
	PopupMenu	getMenuForIndex( int menuIndex, const String& /*menuName*/ ) override;
	void		menuItemSelected( int /*menuItemID*/, int /*topLevelMenuIndex*/ ) override;

	//==============================================================================
	ApplicationCommandTarget* getNextCommandTarget() override;
	void getAllCommands( Array<CommandID>& c ) override;
	void getCommandInfo( CommandID commandID, ApplicationCommandInfo& result ) override;
	bool perform( const InvocationInfo& info ) override;


private:

	//==============================================================================
	void timerCallback() override;

	void mouseDown (const MouseEvent& e) override;
	void mouseDrag (const MouseEvent& e) override;

	Matrix3D<float> MainComponent::getProjectionMatrix( float scale ) const;
	Matrix3D<float> MainComponent::getViewMatrix( float rotation ) const;



	//==============================================================================
	// Your private member variables go here...
	ApplicationCommandManager commandManager;
	std::unique_ptr<MenuBarComponent> menuBar;
    std::unique_ptr<OpenGLGraphicsContextCustomShader> shader;
	Draggable3DOrientation draggableOrientation;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( MainComponent )
};
