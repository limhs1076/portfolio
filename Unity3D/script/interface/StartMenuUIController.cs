using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class StartMenuUIController : MonoBehaviour
{

    public Button bStart;
    public Button bExit;
    public string nextGameScene;

    // Start is called before the first frame update
    void Start()
    {
        bStart.onClick.AddListener(StartGame);
        bExit.onClick.AddListener(ExitGame);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void StartGame()
    {
        //Debug.Log("Game start");
        int index = SceneManager.GetActiveScene().buildIndex;
        SceneManager.LoadSceneAsync(index + 1);
    }
    public void ExitGame()
    {
        //Debug.Log("Game exit");
        #if UNITY_EDITOR
            UnityEditor.EditorApplication.isPlaying = false;
        #else
            Application.Quit();
        #endif
    }
}
