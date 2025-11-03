#include "JuceHeader.h"
#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &playButton,  &muteButton ,&forward10sButton, &backward10sButton , &sleepTimerButton,&repeatButton ,&goToStartButton, &goToEndButton,&set_AButton,&set_BButton ,&favoriteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
        playButton.setClickingTogglesState(true);

        btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF3A2F45));
        btn->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFFFF2D8B));
        btn->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    }

    addAndMakeVisible(titleLabel);
    addAndMakeVisible(artistLabel);
    addAndMakeVisible(durationLabel);
    addAndMakeVisible(addToPlaylistButton);
    addAndMakeVisible(playlistBox);
    addToPlaylistButton.addListener(this);

    addToPlaylistButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF3A2F45));
    addToPlaylistButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFFFF2D8B));
    addToPlaylistButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);


    playlistBox.setColour(juce::ListBox::backgroundColourId, juce::Colour(0xFF1B1525));
    playlistBox.setColour(juce::ListBox::outlineColourId, juce::Colour(0xFFFF2D8B));

    playlistModel = std::make_unique<PlaylistModel>(playlistFiles, PlayerAudio1, *this);
    playlistBox.setModel(playlistModel.get());



    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    volumeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    volumeLabel.setJustificationType(juce::Justification::centredLeft);


    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    volumeSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xFFFF2D8B));
    volumeSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(0xFF2A2035));
    volumeSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xFFFF80AB));
    addAndMakeVisible(volumeSlider);


    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    speedLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    speedLabel.setJustificationType(juce::Justification::centredLeft);


    speedSlider.setRange(0.5, 2.0, 0.05);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    speedSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    speedSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xFFFF2D8B));
    speedSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(0xFF2A2035));
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xFFFF80AB));
    addAndMakeVisible(speedSlider);

    positionSlider.setRange(0.0, 1.0);
    positionSlider.setValue(0.0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    PlayerAudio1.Myfav.addListener(this);
    addAndMakeVisible(PlayerAudio1.Myfav);


    startTimer(50);



    addAndMakeVisible(timeLabel);
    timeLabel.setText("0:00 / 0:00", juce::dontSendNotification);
    timeLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    timeLabel.setJustificationType(juce::Justification::centred);
    timeLabel.setColour(juce::Label::textColourId, juce::Colours::white);


    useProgressBar = true;
    waveformComponent = std::make_unique<WaveformDisplay>(PlayerAudio1.getThumbnail(), currentPosition);
    addAndMakeVisible(waveformComponent.get());

	
}

PlayerGUI::~PlayerGUI()
{
    stopTimer();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF1B1525));
    g.setColour(juce::Colour(0xFF241C2F));
    g.fillRoundedRectangle(getLocalBounds().reduced(10).toFloat(), 10.0f);
}

void PlayerGUI::resized()
{
    auto area = getLocalBounds().reduced(15);

    auto waveArea = area.removeFromTop(140);
    waveformComponent->setBounds(waveArea.withSizeKeepingCentre(waveArea.getWidth() * 0.95f, 140));

    positionSlider.setBounds(area.removeFromTop(30).reduced(10));

    timeLabel.setBounds(area.removeFromTop(30).reduced(10));

    auto infoRow = area.removeFromTop(35);
    titleLabel.setBounds(infoRow.removeFromLeft(infoRow.getWidth() * 0.4f).reduced(5));
    artistLabel.setBounds(infoRow.removeFromLeft(infoRow.getWidth() * 0.3f).reduced(5));
    durationLabel.setBounds(infoRow.reduced(5));

    auto ctrlRow = area.removeFromTop(65);
    int btnW = 75;
    int gap = 8;

    loadButton.setBounds(ctrlRow.removeFromLeft(btnW).reduced(5));
    ctrlRow.removeFromLeft(gap);
    playButton.setBounds(ctrlRow.removeFromLeft(btnW).reduced(5));
    ctrlRow.removeFromLeft(gap);

    goToStartButton.setBounds(ctrlRow.removeFromLeft(btnW).reduced(5));
    ctrlRow.removeFromLeft(gap);
    goToEndButton.setBounds(ctrlRow.removeFromLeft(btnW).reduced(5));
    ctrlRow.removeFromLeft(gap);
    repeatButton.setBounds(ctrlRow.removeFromLeft(btnW).reduced(5));
    ctrlRow.removeFromLeft(gap);

    muteButton.setBounds(ctrlRow.removeFromLeft(btnW).reduced(5));
    ctrlRow.removeFromLeft(gap);
    backward10sButton.setBounds(ctrlRow.removeFromLeft(btnW).reduced(5));
    ctrlRow.removeFromLeft(gap);
    forward10sButton.setBounds(ctrlRow.removeFromLeft(btnW).reduced(5));
    ctrlRow.removeFromLeft(gap);
    sleepTimerButton.setBounds(ctrlRow.removeFromLeft(btnW).reduced(5));

    auto slidersArea = area.removeFromTop(100).reduced(5);

    volumeLabel.setBounds(slidersArea.removeFromTop(20));
    volumeSlider.setBounds(slidersArea.removeFromTop(20));

    speedLabel.setBounds(slidersArea.removeFromTop(20));
    speedSlider.setBounds(slidersArea.removeFromTop(20));

    auto secButtonsRow = area.removeFromTop(40).reduced(5);
    int extraBtnW = 80;
    int extraGap = 10;

    set_AButton.setBounds(secButtonsRow.removeFromLeft(extraBtnW).reduced(2));
    secButtonsRow.removeFromLeft(extraGap);
    set_BButton.setBounds(secButtonsRow.removeFromLeft(extraBtnW).reduced(2));
    secButtonsRow.removeFromLeft(extraGap);
    favoriteButton.setBounds(secButtonsRow.removeFromLeft(extraBtnW).reduced(2));
    secButtonsRow.removeFromLeft(extraGap);
    PlayerAudio1.Myfav.setBounds(secButtonsRow.removeFromLeft(40).reduced(2));

    auto playlistArea = area.reduced(5);

    addToPlaylistButton.setBounds(playlistArea.removeFromTop(30).reduced(5));

    playlistBox.setBounds(playlistArea.reduced(5));

}

void PlayerGUI::buttonClicked(juce::Button* button) {
    if (button == &loadButton) {
        fileChooser = std::make_unique<juce::FileChooser>("Select an audio file...", juce::File{}, "*.wav;*.mp3");
        fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc) {
                auto file = fc.getResult();
                if (file.existsAsFile() && PlayerAudio1.loadFile(file)) {
                    double length = PlayerAudio1.getLength();
                    timeLabel.setText("0:00 / " + secondsToTime(length), juce::dontSendNotification);
                    currentPosition = 0.0;
                    if (useProgressBar) {
                        waveformComponent = std::make_unique<WaveformDisplay>(PlayerAudio1.getThumbnail(), currentPosition);

                        addAndMakeVisible(waveformComponent.get());
                        PlayerAudio1.currentFile = file;
                        PlayerAudio1.loadFile(file);
                        currentPosition = 0.0;
                        PlayerAudio1.loadFile(file);
                        titleLabel.setText("Title: " + PlayerAudio1.getTitle(), juce::dontSendNotification);
                        artistLabel.setText("Artist: " + PlayerAudio1.getArtist(), juce::dontSendNotification);
                        durationLabel.setText("Duration: " + PlayerAudio1.getDuration(), juce::dontSendNotification);
                        PlayerAudio1.play();
                        playButton.setButtonText("pause");
                        isPlaying = true;

                    }
                    positionSlider.setRange(0.0, PlayerAudio1.getLength());
                    positionSlider.setValue(0.0);
                    favoriteButton.setButtonText("My Favorite");
                    if (PlayerAudio1.favorite.contains(file)) {
                        favoriteButton.setButtonText("UnFavorite");

                    }
                    else {
                        favoriteButton.setButtonText("My Favorite");
                    }

                    resized();
                }
            });
    }

    else if (button == &playButton)
    {

        if (PlayerAudio1.isPlaying())
        {
            PlayerAudio1.pause();
            playButton.setButtonText("Play");


        }
        else
        {
            startTimer(50);
            PlayerAudio1.play();
            playButton.setButtonText("Pause");

        }
    }

    else if (button == &repeatButton)
    {
        PlayerAudio1.repeat();
        if (repeatButton.getButtonText() == "Repeat") {

            repeatButton.setButtonText("UnRepeat");
        }
        else {
            repeatButton.setButtonText("Repeat");
        }
    }
    else if (button == &muteButton) 
        {
            bool shouldMute = !PlayerAudio1.isMuted();
            PlayerAudio1.setMuted(shouldMute);
            muteButton.setButtonText(shouldMute ? "Unmute" : "Mute");
        }
    
    else if (button == &backward10sButton) {
        if (currentPosition >= 10) {
            PlayerAudio1.setPosition(PlayerAudio1.getPosition() - 10.0);
            currentPosition = PlayerAudio1.getPosition() - 10.0;
            if (PlayerAudio1.isPlaying()) PlayerAudio1.play();
        }

        else if (currentPosition < 10) {
            PlayerAudio1.setPosition(0.0);
            currentPosition = PlayerAudio1.getPosition() - currentPosition;
            if (PlayerAudio1.isPlaying()) PlayerAudio1.play();
        }
    }

    else if (button == &forward10sButton)
    {
        double newPosition = PlayerAudio1.getPosition() + 10.0;
        double length = PlayerAudio1.getLength();
        PlayerAudio1.setPosition(newPosition);
        currentPosition = newPosition;
        if (PlayerAudio1.isPlaying()) PlayerAudio1.play();
    }
    else if (button == &sleepTimerButton)
    {
        sleepTimerEnabled = !sleepTimerEnabled;
        sleepTimerButton.setButtonText(sleepTimerEnabled ? "Sleep : On" : "Sleep : Off");

        if (sleepTimerEnabled) sleepTimerCounter = 0;
        else sleepTimerCounter = 0;
    }
    else if (button == &goToStartButton)
    {
        PlayerAudio1.setPosition(0.0);
        PlayerAudio1.play();
        playButton.setButtonText("Pause");

    }
    else if (button == &goToEndButton)
    {
        double length = PlayerAudio1.getLength();
        PlayerAudio1.setPosition(length);
        playButton.setToggleState(false, juce::dontSendNotification);
        playButton.setButtonText("Play");

    }
    else if (button == &addToPlaylistButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select audio files to add to playlist...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile()) {
                    playlistFiles.push_back(file);
                    playlistBox.updateContent();
                    repaint();



                }
            });
    }

    else if (button == &set_AButton) {
        A = positionSlider.getValue();
        repeat = false;


    }

    else if (button == &set_BButton) {
        B = positionSlider.getValue();
        repeat = !repeat;

    }

    else if (button == &favoriteButton) {
        PlayerAudio1.fav();

        if (PlayerAudio1.favorite.contains(PlayerAudio1.currentFile)) {
            favoriteButton.setButtonText("UnFavorite");

        }
        else {
            favoriteButton.setButtonText("My Favorite");
        }
    }

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider) PlayerAudio1.setGain((float)slider->getValue());
    else if (slider == &speedSlider) PlayerAudio1.setSpeed(slider->getValue());
    else if (slider == &positionSlider)
    {
        PlayerAudio1.setPosition(positionSlider.getValue());
    }
}

void PlayerGUI::timerCallback()
{
    positionSlider.setValue(PlayerAudio1.getPosition(), juce::dontSendNotification);
    if (repeat && PlayerAudio1.getPosition() >= B) {
        PlayerAudio1.setPosition(A);
        PlayerAudio1.play();

    }
    currentPosition = PlayerAudio1.getPosition();
    double len = PlayerAudio1.getLength();
    timeLabel.setText(secondsToTime(currentPosition) + " / " + secondsToTime(len), juce::dontSendNotification);

    if (waveformComponent)
        waveformComponent->repaint();

    if (sleepTimerEnabled && PlayerAudio1.isPlaying())
    {
        sleepTimerCounter++;
        if (sleepTimerCounter >= sleepTimerDuration)
        {
            PlayerAudio1.pause();
            sleepTimerEnabled = false;
            sleepTimerCounter = 0;
            sleepTimerButton.setButtonText("Sleep Off");

        }
    }
    if (PlayerAudio1.getPosition() >= PlayerAudio1.getLength())
    {
        playButton.setButtonText("Play");
    }
}
juce::String PlayerGUI::secondsToTime(double s)
{
    int m = (int)s / 60;
    int sec = (int)s % 60;
    return juce::String(m) + ":" + (sec < 10 ? "0" : "") + juce::String(sec);
}

void PlayerGUI::comboBoxChanged(juce::ComboBox* comboBox)
{
    PlayerAudio1.currentFile = PlayerAudio1.favorite[comboBox->getSelectedItemIndex()];
    PlayerAudio1.loadFile(PlayerAudio1.favorite[comboBox->getSelectedItemIndex()]);
    titleLabel.setText("Title: " + PlayerAudio1.getTitle(), juce::dontSendNotification);
    artistLabel.setText("Artist: " + PlayerAudio1.getArtist(), juce::dontSendNotification);
    durationLabel.setText("Duration: " + PlayerAudio1.getDuration(), juce::dontSendNotification);
    if (PlayerAudio1.favorite.contains(PlayerAudio1.currentFile)) {
        favoriteButton.setButtonText("UnFavorite");

    }
    else {
        favoriteButton.setButtonText("My Favorite");
    }

    positionSlider.setRange(0.0, PlayerAudio1.getLength());


    PlayerAudio1.play();
    playButton.setButtonText("Pause");

}


void PlayerGUI::prepareToPlay(int samples, double rate) { PlayerAudio1.prepareToPlay(samples, rate); }
void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& b) { PlayerAudio1.getNextAudioBlock(b); }
void PlayerGUI::releaseResources() { PlayerAudio1.releaseResources(); stopTimer(); }