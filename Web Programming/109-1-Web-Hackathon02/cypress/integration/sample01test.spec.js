describe('Hackathon 2 Test', () => {
  // 1st test
  it('[Grid_9x9] Load the game correctly (30%)', () => {
    cy.visit('/')
    cy.get('#demo-simple-select-outlined').click()
    cy.get('.MuiMenuItem-root').contains('sample01.json').click()

    cy.get('div[id="grid-1*2"]').should('have.text', '4')
    cy.get('div[id="grid-0*4"]').should('have.text', '2')
    cy.get('div[id="grid-2*6"]').should('have.text', '1')
    cy.get('div[id="grid-4*1"]').should('have.text', '9')
    cy.get('div[id="grid-4*4"]').should('have.text', '')
    cy.get('div[id="grid-3*8"]').should('have.text', '7')
    cy.get('div[id="grid-8*0"]').should('have.text', '4')
    cy.get('div[id="grid-8*5"]').should('have.text', '1')
    cy.get('div[id="grid-6*6"]').should('have.text', '8')
  })

  // 4th test
  it('[Input Functions] Complete the key board inupt function (20%)', () => {
    cy.visit('/')
    cy.get('#demo-simple-select-outlined').click()
    cy.get('.MuiMenuItem-root').contains('sample01.json').click()
    
    cy.get('div[id="grid-4*4"]').click().type('7').should('have.text', '7')
    cy.get('div[id="grid-5*4"]').click().type('5').should('have.text', '5')
  })
})
