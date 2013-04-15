#pragma once
#include <stdlib.h>
#include <vcclr.h>

namespace TennisTracking {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Start : public System::Windows::Forms::Form
	{
	public:
		Start(int& option, char *file1, int& alpha)
		{
			InitializeComponent();
			this->option = &option;
			this->file1 = file1;
			this->alpha = &alpha;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Start()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::TextBox^  filename1;
	private: System::Windows::Forms::Button^  button_browse;
	private: System::Windows::Forms::Button^  button_run;
			 int *option, *alpha;
			 char *file1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label2;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->filename1 = (gcnew System::Windows::Forms::TextBox());
			this->button_browse = (gcnew System::Windows::Forms::Button());
			this->button_run = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(43, 48);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(69, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Pick a video:";
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// filename1
			// 
			this->filename1->Location = System::Drawing::Point(118, 45);
			this->filename1->Name = L"filename1";
			this->filename1->Size = System::Drawing::Size(260, 20);
			this->filename1->TabIndex = 1;
			// 
			// button_browse
			// 
			this->button_browse->Location = System::Drawing::Point(396, 42);
			this->button_browse->Name = L"button_browse";
			this->button_browse->Size = System::Drawing::Size(75, 23);
			this->button_browse->TabIndex = 2;
			this->button_browse->Text = L"Browse...";
			this->button_browse->UseVisualStyleBackColor = true;
			this->button_browse->Click += gcnew System::EventHandler(this, &Start::button_browse_Click);
			// 
			// button_run
			// 
			this->button_run->Location = System::Drawing::Point(353, 125);
			this->button_run->Name = L"button_run";
			this->button_run->Size = System::Drawing::Size(75, 23);
			this->button_run->TabIndex = 3;
			this->button_run->Text = L"Track!";
			this->button_run->UseVisualStyleBackColor = true;
			this->button_run->Click += gcnew System::EventHandler(this, &Start::button_run_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(118, 94);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(49, 20);
			this->textBox1->TabIndex = 4;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Start::textBox1_TextChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(54, 97);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(37, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Alpha:";
			// 
			// Start
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(508, 175);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button_run);
			this->Controls->Add(this->button_browse);
			this->Controls->Add(this->filename1);
			this->Controls->Add(this->label1);
			this->Name = L"Start";
			this->Text = L"Tennis Ball Tracking";
			this->Load += gcnew System::EventHandler(this, &Start::Start_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button_browse_Click(System::Object^  sender, System::EventArgs^  e) {
		if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			this->filename1->Text = openFileDialog1->FileName;
			pin_ptr<const wchar_t> wch = PtrToStringChars(openFileDialog1->FileName);
			size_t origsize = wcslen(wch) + 1;
			size_t convertedChars = 0;
			wcstombs_s(&convertedChars, file1, origsize, wch, _TRUNCATE);
		}
	}
	private: System::Void button_run_Click(System::Object^  sender, System::EventArgs^  e) {
		*option = 1;
		Close();
	}
private: System::Void Start_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			*alpha = System::Int32::Parse(textBox1->Text);
		 }
};
}

