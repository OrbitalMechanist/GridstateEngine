////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_RIVECONTROL_H__
#define __APP_RIVECONTROL_H__


#include <NsCore/Noesis.h>
#include <NsCore/String.h>
#include <NsApp/RiveBaseApi.h>
#include <NsGui/FrameworkElement.h>
#include <NsMath/Transform.h>

#include <memory>


namespace Noesis
{
struct Uri;
template<class T> class UICollection;
enum Stretch: int32_t;
}

namespace rive
{
class File;
class ArtboardInstance;
class Scene;
}

namespace NoesisApp
{

class RiveInput;
class RiveFactory;
class RiveRenderer;
typedef Noesis::UICollection<RiveInput> RiveInputCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
enum RiveSourceInputType
{
    RiveSourceInputType_Bool,
    RiveSourceInputType_Number,
    RiveSourceInputType_Trigger
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)
NS_CLANG_WARNING_DISABLE("-Wdocumentation")

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Renders a `Rive <https://rive.app/>`_ scene.
///
/// The scene can contain inputs to control the state machine and animations. This control exposes
/// a list of RiveInput to set the values of those inputs in XAML.
///
/// .. code-block:: xml
///
///  <Grid
///    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///    xmlns:noesis="clr-namespace:NoesisGUIExtensions;assembly=Noesis.GUI.Extensions">
///    <noesis:RiveControl Source="player.riv">
///      <noesis:RiveInput InputName="Life" InputValue="{Binding PlayerLife}"/>
///    </noesis:RiveControl>
///  </Grid>
///
/// The UI can fire triggers defined in the Rive scene using the interactivity RiveTriggerAction.
///
/// .. code-block:: xml
///
///  <Grid
///    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///    xmlns:b="http://schemas.microsoft.com/xaml/behaviors"
///    xmlns:noesis="clr-namespace:NoesisGUIExtensions;assembly=Noesis.GUI.Extensions">
///    <noesis:RiveControl x:Name="rive" Source="player.riv"/>
///    <Button Content="{Binding PlayerName}">
///      <b:Interaction.Triggers>
///        <b:EventTrigger EventName="Click">
///          <noesis:RiveTriggerAction TargetName="rive" TriggerName="Fire"/>
///        </b:EventTrigger>
///      </b:Interaction.Triggers>
///    </Button>
///  </Grid>
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_APP_RIVEBASE_API RiveControl: public Noesis::FrameworkElement
{
public:
    RiveControl();
    ~RiveControl();

    /// Gets or sets the Rive scene source
    //@{
    const Noesis::Uri& GetSource() const;
    void SetSource(const Noesis::Uri& value);
    //@}

    /// Gets the name of the current state in the state machine
    //@{
    const char* GetState() const;
    //@}

    /// Gets or sets a Stretch value that describes how the Rive scene fills its allocated space
    //@{
    Noesis::Stretch GetStretch() const;
    void SetStretch(Noesis::Stretch stretch);
    //@}

    /// Gets the collection of inputs that will control the state machine
    RiveInputCollection* GetInputs() const;

    /// Sets the value of an input in the Rive scene
    void SetInputValue(const char* name, Noesis::BaseComponent* value);

    /// Fires a trigger input in the Rive scene
    void FireInputTrigger(const char* name);

    /// Gets the number of available inputs in the associated Rive scene
    uint32_t GetSourceInputCount() const;

    /// Gets information about the specified input
    const char* GetSourceInput(uint32_t index, RiveSourceInputType& type) const;

public:
    /// Dependency properties
    //@{
    static const Noesis::DependencyProperty* SourceProperty;
    static const Noesis::DependencyProperty* StateProperty;
    static const Noesis::DependencyProperty* StretchProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    void OnInit() override;
    //@}

    /// From Visual
    //@{
    void OnConnectToView(Noesis::IView* view) override;
    void OnDisconnectFromView() override;
    //@}

    /// From UIElement
    //@{
    void OnMouseMove(const Noesis::MouseEventArgs& e) override;
    void OnMouseDown(const Noesis::MouseButtonEventArgs& e) override;
    void OnMouseUp(const Noesis::MouseButtonEventArgs& e) override;
    void OnRender(Noesis::DrawingContext* context) override;
    //@}

    /// From FrameworkElement
    //@{
    Noesis::Size MeasureOverride(const Noesis::Size& availableSize) override;
    Noesis::Size ArrangeOverride(const Noesis::Size& finalSize) override;
    void CloneOverride(Noesis::FrameworkElement* clone,
        Noesis::FrameworkTemplate* template_) const override;
    //@}

private:
    void EnsureInputs();
    void OnInputsChanged(Noesis::BaseComponent* sender,
        const Noesis::NotifyCollectionChangedEventArgs& e);

    void UnregisterInputs();

    void LoadSource(const Noesis::Uri& uri);
    void OnStateChanged(const char* state);

    void Advance(Noesis::IView* view);

private:
    Noesis::Ptr<RiveInputCollection> mInputs;

    std::unique_ptr<RiveFactory> mRiveFactory;
    std::unique_ptr<RiveRenderer> mRiveRenderer;
    std::unique_ptr<rive::File> mRiveFile;
    std::unique_ptr<rive::ArtboardInstance> mRiveArtboard;
    std::unique_ptr<rive::Scene> mRiveScene;

    uint64_t mTicks;
    Noesis::Transform2 mToRiveScene;

    NS_DECLARE_REFLECTION(RiveControl, Noesis::FrameworkElement)
};

NS_WARNING_POP

}


#endif
