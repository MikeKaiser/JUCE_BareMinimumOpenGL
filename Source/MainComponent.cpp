/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent()
{
	menuBar.reset( new MenuBarComponent( this ) );
	addAndMakeVisible( menuBar.get() );
	setApplicationCommandManagerToWatch( &commandManager );
	commandManager.registerAllCommandsForTarget( this );
	addKeyListener( commandManager.getKeyMappings() );   // this lets the command manager use keypresses that arrive in our window to send out commands
	menuBar->setVisible( true );

	// Make sure you set the size of the component after
	// you add any child components.
	setSize( 800, 600 );
}

MainComponent::~MainComponent()
{
	// This shuts down the GL system and stops the rendering calls.
	shutdownOpenGL();
	shader.reset();
}

//==============================================================================
void MainComponent::initialise()
{
	// Initialise GL objects for rendering here.
}

void MainComponent::shutdown()
{
	// Free any GL objects created for rendering here.
}


template <typename Type>
Matrix3D<Type> FromFrustum (Type l, Type r, Type b, Type t, Type n, Type f) noexcept
{
    return {	(Type (2)) / (r - l), 0, 0, 0,
                0, (Type (2)) / (t - b), 0, 0,
                0, 0, (Type (-2) / (f - n)), 0,
                -(r+l)/(r-l), -(t+b)/(t-b), -(f+n)/(f-n), 1 };
}


Matrix3D<float> MainComponent::getProjectionMatrix( float scale ) const
{
    auto w = 1.0f / (scale + 0.1f);
    auto h = w * getLocalBounds().toFloat().getAspectRatio (false);
    return FromFrustum (-w, w, -h, h, 0.001f, 100.0f);
}

Matrix3D<float> MainComponent::getViewMatrix( float rotation ) const
{
	auto translation = Vector3D<float> (0.0f, 0.0f, -10.0f);
    auto viewMatrix = draggableOrientation.getRotationMatrix() * translation;
    auto rotationMatrix = Matrix3D<float>::rotation ({ rotation, rotation, -0.3f });
    return rotationMatrix * viewMatrix;
}

Matrix3D<double> ConvertFToD( const Matrix3D<float> & src )
{
	Matrix3D<double> dst;

	for( int i=0; i<16; ++i )
		dst.mat[i] = (double)src.mat[i];

	return dst;
}

void MainComponent::render()
{
	// This clears the context with a black background.
	OpenGLHelpers::clear( Colours::grey );

	// Add your rendering code here...
	Matrix3D<float> proj = getProjectionMatrix( 1.0f );
	Matrix3D<float> view = getViewMatrix( 0.0f );

	float desktopScale = (float) openGLContext.getRenderingScale();
	int w = roundToInt (desktopScale * getWidth());
	int h = roundToInt (desktopScale * getHeight());
	glViewport (0, 0, w, h);

	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( proj.mat );
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( view.mat );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
    glDisable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable( GL_TEXTURE_2D );

	glBegin( GL_TRIANGLES );
	{
		glColor3f( 1, 0, 0 );	glVertex3f( 1, 0, 0 );
		glColor3f( 0, 1, 0 );	glVertex3f( 0, 1, 0 );
		glColor3f( 0, 0, 1 );	glVertex3f( 0, 0, 1 );
	}
	glEnd();

    // Reset the element buffers so child Components draw correctly
    openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
    openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
}

//==============================================================================
void MainComponent::paint( Graphics& g )
{
	// You can add your component specific drawing code here!
	// This will draw over the top of the openGL background.
	g;
}

void MainComponent::resized()
{
	// This is called when the MainComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
	auto b = getLocalBounds();
	menuBar->setBounds( b.removeFromTop( LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight() ) );
    draggableOrientation.setViewport (getLocalBounds());
}

StringArray MainComponent::getMenuBarNames()
{
	return { "File", "Edit", "Help" };
}

PopupMenu MainComponent::getMenuForIndex( int menuIndex, const String& /*menuName*/ )
{
	PopupMenu menu;
	if( menuIndex == 0 )
	{
		menu.addCommandItem( &commandManager, CommandIDs::File_Open );
		menu.addCommandItem( &commandManager, CommandIDs::File_Save );
		menu.addCommandItem( &commandManager, CommandIDs::File_SaveAs );
		menu.addCommandItem( &commandManager, CommandIDs::File_Exit );
	}
	else if( menuIndex == 1 )
	{
		menu.addCommandItem( &commandManager, CommandIDs::Edit_Cut );
		menu.addCommandItem( &commandManager, CommandIDs::Edit_Copy );
		menu.addCommandItem( &commandManager, CommandIDs::Edit_Paste );
	}
	else if( menuIndex == 2 )
	{
		menu.addCommandItem( &commandManager, CommandIDs::Help_About );
	}

	return menu;
}

void MainComponent::menuItemSelected( int /*menuItemID*/, int /*topLevelMenuIndex*/ )
{
}

ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
	return nullptr;
}

void MainComponent::getAllCommands( Array<CommandID>& c )
{
	Array<CommandID> commands{	CommandIDs::File_Open,
								CommandIDs::File_Save,
								CommandIDs::File_SaveAs,
								CommandIDs::File_Exit,
								CommandIDs::Edit_Cut,
								CommandIDs::Edit_Copy,
								CommandIDs::Edit_Paste,
								CommandIDs::Help_About,
							};
	c.addArray( commands );
}

void MainComponent::getCommandInfo( CommandID commandID, ApplicationCommandInfo& result )
{
	result.setTicked( false );
	switch( commandID )
	{
		case CommandIDs::File_Open:
			result.setInfo( "Open...", "Open a File.", "FileMenu", 0 );
			result.addDefaultKeypress( 'o', ModifierKeys::ctrlModifier );
			break;
		case CommandIDs::File_Save:
			result.setInfo( "Save...", "Save File (Replace Contents).", "FileMenu", 0 );
			result.addDefaultKeypress( 's', ModifierKeys::ctrlModifier );
			break;
		case CommandIDs::File_SaveAs:
			result.setInfo( "Save As...", "Save to a New File.", "FileMenu", 0 );
			result.addDefaultKeypress( 's', ModifierKeys::ctrlModifier | ModifierKeys::altModifier );
			break;
		case CommandIDs::File_Exit:
			result.setInfo( "Exit", "Close the Program", "FileMenu", 0 );
			result.addDefaultKeypress( KeyPress::F4Key, ModifierKeys::altModifier );
			break;
		case CommandIDs::Edit_Cut:
			result.setInfo( "Cut", "Cut", "EditMenu", 0 );
			result.addDefaultKeypress( 'x', ModifierKeys::ctrlModifier );
			break;
		case CommandIDs::Edit_Copy:
			result.setInfo( "Copy", "Copy", "EditMenu", 0 );
			result.addDefaultKeypress( 'c', ModifierKeys::ctrlModifier );
			break;
		case CommandIDs::Edit_Paste:
			result.setInfo( "Paste", "Paste", "EditMenu", 0 );
			result.addDefaultKeypress( 'v', ModifierKeys::ctrlModifier );
			break;
		case CommandIDs::Help_About:
			result.setInfo( "About", "About", "AboutMenu", 0 );
			result.addDefaultKeypress( KeyPress::F1Key, ModifierKeys::shiftModifier );
			break;
		default:
			break;
	}
}

bool MainComponent::perform( const InvocationInfo& info )
{
	switch( info.commandID )
	{
		case CommandIDs::File_Open:
			break;
		case CommandIDs::File_Save:
			break;
		case CommandIDs::File_SaveAs:
			break;
		case CommandIDs::File_Exit:
			break;
		case CommandIDs::Edit_Cut:
			break;
		case CommandIDs::Edit_Copy:
			break;
		case CommandIDs::Edit_Paste:
			break;
		case CommandIDs::Help_About:
			break;
		default:
			return false;
	}

	return true;
}

void MainComponent::timerCallback()
{
    stopTimer();
    //fragmentCode = fragmentDocument.getAllContent();
    repaint();
}

void MainComponent::mouseDown (const MouseEvent& e)
{
    draggableOrientation.mouseDown (e.getPosition());
}

void MainComponent::mouseDrag (const MouseEvent& e)
{
    draggableOrientation.mouseDrag (e.getPosition());
}
