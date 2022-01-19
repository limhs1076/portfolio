using System.Collections;
using System.Collections.Generic;
using UnityEngine;
[RequireComponent(typeof(MeshRenderer))]
[RequireComponent(typeof(TextMesh))]
public class TextView : MonoBehaviour
{
    private MeshRenderer meshRenderer;
    private TextMesh textMesh;
    private Coroutine myonCor;
    private Coroutine myoffCor;
    // Start is called before the first frame update
    void Start()
    {
        meshRenderer = this.GetComponent<MeshRenderer>();
        textMesh = this.GetComponent<TextMesh>();
        meshRenderer.enabled = false;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    IEnumerator TextViewFadeIn() {
        meshRenderer.enabled = true;
        Debug.Log("텍스트 보여줘");
        while(textMesh.color.a <= 250) {
            textMesh.color = new Color(textMesh.color.r,textMesh.color.g,textMesh.color.b,Mathf.Lerp(0, 256, Time.deltaTime ));
            //Debug.Log("value of meshColor: "+textMesh.color.a);
            yield return new WaitForEndOfFrame();
        }
        myoffCor = null;

    }
    IEnumerator TextViewFadeOut() {

        while (textMesh.color.a >= 10) {
            textMesh.color = new Color(textMesh.color.r, textMesh.color.g, textMesh.color.b, Mathf.Lerp(256, 0, Time.deltaTime ));
            yield return new WaitForEndOfFrame();
        }
        meshRenderer.enabled = false;

        myonCor = null;


    }
    private void OnTriggerEnter(Collider other) {
        if (other.CompareTag("Player")) {
            if(myonCor == null)
                myonCor = StartCoroutine(TextViewFadeIn());
        }
    }
    private void OnTriggerExit(Collider other) {
        if (other.CompareTag("Player")) {
            if(myoffCor == null)
                myoffCor = StartCoroutine(TextViewFadeOut());
        }
    }
}
