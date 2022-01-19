using System.Collections;
using UnityEngine;
using UnityEditor;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityStandardAssets.Characters.FirstPerson;

public class UIController : MonoBehaviour
{
    private bool paused = false;    // ESC로 게임 일시정지 되었는지

    private GameObject player;
    private PlayerInteraction playerInteraction;
    private FirstPersonController firstPersonController;
    private Image[] abilityBox;

    public Text tAbility;
    public Button bResume;
    public Button bExit;
    public GameObject menuPanel;
    public GameObject abilityUILayout;    

    private void Start()
    {
        player = GameObject.FindWithTag("Player");
        firstPersonController = player.GetComponent<FirstPersonController>();
        playerInteraction = player.transform.GetChild(0).GetComponent<PlayerInteraction>();
        abilityBox = new Image[5];

        for (int i=0; i<5; i++)
        {
            abilityBox[i] = abilityUILayout.transform.GetChild(i).GetComponent<Image>();
        }

        bResume.onClick.AddListener(OnClickResumeButton);
        bExit.onClick.AddListener(OnClickExitButton);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            GameMenu();
        }
    }

    public void GameMenu()
    {
        if (paused == false)
        {
            Debug.Log("game puased");

            paused = true;
            firstPersonController.enabled = false;
            menuPanel.SetActive(true);
            Cursor.visible = true;
            Cursor.lockState = CursorLockMode.None;
            Time.timeScale = 0.0f;
        }
        else
        {
            Debug.Log("game running");

            paused = false;
            firstPersonController.enabled = true;
            menuPanel.SetActive(false);
            Cursor.visible = false;
            Cursor.lockState = CursorLockMode.Locked;
            Time.timeScale = 1.0f;
        }
    }

    public void ChangeAbilityColor(Ability ability)
    {
        // 사용할 수 있는 능력만 흰색으로 칠함
        for (int i=0; i<5; i++)
        {
            if (playerInteraction.canUseAbility[i] == true)
            {
                abilityBox[i].color = Color.white;
            }
        }

        switch(ability)
        {
            case Ability.magnetism:
                abilityBox[0].color = Color.green;
                tAbility.text = "Magnetism";
                break;

            case Ability.gravity:
                abilityBox[1].color = Color.green;
                tAbility.text = "Gravity";
                break;

            case Ability.acceleration:
                abilityBox[2].color = Color.green;
                tAbility.text = "Acceleration";
                break;

            case Ability.invisible:
                abilityBox[3].color = Color.green;
                tAbility.text = "Invisible";
                break;

            case Ability.elasticity:
                abilityBox[4].color = Color.green;
                tAbility.text = "Elasticity";
                break;

            default:
                break;
        }
    }

    public void OnClickResumeButton()
    {
        Debug.Log("Resume pussed");
        GameMenu();
    }

    public void OnClickExitButton()
    {
        Debug.Log("Game exit to start menu");
        Time.timeScale = 1.0f;
        SceneManager.LoadScene("Start Scene");
    }
}

 
//public class SceneAttribute : PropertyAttribute {
//}

//[CustomPropertyDrawer(typeof(SceneAttribute))]
//public class SceneDrawer : PropertyDrawer {

//    public override void OnGUI(Rect position, SerializedProperty property, GUIContent label) {

//        if (property.propertyType == SerializedPropertyType.String) {
//            var sceneObject = GetSceneObject(property.stringValue);
//            var scene = EditorGUI.ObjectField(position, label, sceneObject, typeof(SceneAsset), true);
//            if (scene == null) {
//                property.stringValue = "";
//            } else if (scene.name != property.stringValue) {
//                var sceneObj = GetSceneObject(scene.name);
//                if (sceneObj == null) {
//                    Debug.LogWarning("The scene " + scene.name + " cannot be used. To use this scene add it to the build settings for the project");
//                } else {
//                    property.stringValue = scene.name;
//                }
//            }
//        } else
//            EditorGUI.LabelField(position, label.text, "Use [Scene] with strings.");
//    }
//    protected SceneAsset GetSceneObject(string sceneObjectName) {
//        if (string.IsNullOrEmpty(sceneObjectName)) {
//            return null;
//        }

//        foreach (var editorScene in EditorBuildSettings.scenes) {
//            if (editorScene.path.IndexOf(sceneObjectName) != -1) {
//                return AssetDatabase.LoadAssetAtPath(editorScene.path, typeof(SceneAsset)) as SceneAsset;
//            }
//        }
//        Debug.LogWarning("Scene [" + sceneObjectName + "] cannot be used. Add this scene to the 'Scenes in the Build' in build settings.");
//        return null;
//    }
//}
