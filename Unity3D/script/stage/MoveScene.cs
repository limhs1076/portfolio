using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
public class MoveScene : MonoBehaviour{
    //public int index;
    
    private void Start() {
    
    }
    private void OnTriggerEnter(Collider other) {
        if (other.CompareTag("Player")) {
            int index = SceneManager.GetActiveScene().buildIndex;
            SceneManager.LoadSceneAsync(index+1);
        }
    }
}
