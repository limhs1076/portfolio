using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnCube : MonoBehaviour{
    [Header("Cube 프리팹 must be 할당!")]
    public GameObject CubePrefab;
    
    private GameObject objCube = null;
    private GameObject ATM;
    private GameObject SpawnEffect;
    private Transform SpawnPoint;
    private TextMesh TextPop;
    private Coroutine SpawnCorutine;
    // Start is called before the first frame update
    void Start(){
        if (CubePrefab == null) {
            Debug.LogError("스포너에 큐브 할당");
            Application.Quit();
        }
        ATM = this.gameObject.transform.Find("ATM").gameObject;

        TextPop = ATM.transform.Find("TextPop").GetComponent<TextMesh>();

        SpawnEffect = this.gameObject.transform.Find("Tube").Find("SpawnEffect").gameObject;
        SpawnEffect.SetActive(false);

        SpawnPoint = this.gameObject.transform.Find("Tube").Find("SpawnPoint");
    }
    public IEnumerator Spawn() {
        SpawnEffect.SetActive(true);
        if(objCube == null) {
            objCube = Instantiate(CubePrefab);
        }
        objCube.SetActive(true);
        objCube.GetComponent<Rigidbody>().useGravity = false;
        objCube.GetComponent<Rigidbody>().velocity = Vector3.zero;
        objCube.transform.position = SpawnPoint.position;
        objCube.GetComponent<BoxCollider>().enabled = false;
        objCube.GetComponent<ColorableObject>().icolor = objCube.GetComponent<ColorableObject>().colorNothing;
        objCube.GetComponent<ColorableObject>().enabled = false;
        objCube.GetComponent<SelectableObject>().enabled = false;
        this.gameObject.GetComponent<AudioSource>().Play();
        yield return new WaitForSeconds(1.0f);
        objCube.GetComponent<Rigidbody>().useGravity = true;
        objCube.GetComponent<BoxCollider>().enabled = true;
        objCube.GetComponent<ColorableObject>().enabled = true;
        objCube.GetComponent<SelectableObject>().enabled = true;
        SpawnEffect.SetActive(false);
        SpawnCorutine = null;
        yield return null;
    }
    public void OnTriggerStay(Collider other) {
        if(other.gameObject.CompareTag("Player") && Input.GetButtonDown("Submit") && SpawnCorutine==null && PlayerRay.isObjectPickUp == false) {
            SpawnCorutine = StartCoroutine(Spawn());
        }
    }
}
